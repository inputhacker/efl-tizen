#include "evas_common_private.h"

#include <assert.h>

typedef struct
{
   char *thread_name;
   Eina_Thread worker;
   Eina_Condition queue_condition;
   Eina_Lock queue_lock;
   Eina_Bool queue_ready;
   Eina_Inarray queue;
   Evas_Thread_Command *queue_cache;
   unsigned int queue_cache_max;

   Eina_Spinlock finish_spinlock;
   Evas_Thread_Command *waiting_cmd;

   volatile int exited;
   Eina_Bool exit_thread;
} Evas_Thread;

static int evas_threads_init_count = 0;

static Evas_Thread evas_thread_software;
static Evas_Thread evas_thread_gl;
static Evas_Thread evas_thread_evgl;

#define SHUTDOWN_TIMEOUT_RESET (0)
#define SHUTDOWN_TIMEOUT_CHECK (1)
#define SHUTDOWN_TIMEOUT (3000)

static Eina_Bool
_shutdown_timeout(double *time, int mode, int timeout_ms)
{
   struct timeval tv;

   gettimeofday(&tv, NULL);

   if ( mode == SHUTDOWN_TIMEOUT_RESET )
     *time = (tv.tv_sec + tv.tv_usec / 1000000.0) * 1000.0;
   return ((tv.tv_sec + tv.tv_usec / 1000000.0) * 1000.0 - (*time)) > timeout_ms ;
}

static void
evas_thread_queue_append(Evas_Thread *ev_thread, Evas_Thread_Command_Cb cb, void *data, Eina_Bool do_flush, Eina_Bool do_finish)
{
   Evas_Thread_Command *cmd;

   eina_lock_take(&ev_thread->queue_lock);

   cmd = eina_inarray_grow(&ev_thread->queue, 1);
   if (cmd)
     {
        cmd->cb = cb;
        cmd->data = data;
     }
   else
     {
        ERR("Out of memory allocating thread command.");
        goto out;
     }

   if (do_finish)
     {
        eina_spinlock_take(&ev_thread->finish_spinlock);
        ev_thread->waiting_cmd = cmd;
        eina_spinlock_release(&ev_thread->finish_spinlock);
     }

   if (do_flush)
     {
        ev_thread->queue_ready = EINA_TRUE;
        eina_condition_signal(&ev_thread->queue_condition);
     }

out:
   eina_lock_release(&ev_thread->queue_lock);
}

EAPI void
evas_thread_cmd_enqueue(Evas_Thread_Command_Cb cb, void *data)
{
   evas_thread_queue_append(&evas_thread_software, cb, data, EINA_FALSE, EINA_FALSE);
}

EAPI void
evas_thread_queue_flush(Evas_Thread_Command_Cb cb, void *data)
{
   evas_thread_queue_append(&evas_thread_software, cb, data, EINA_TRUE, EINA_FALSE);
}

EAPI void
evas_gl_thread_cmd_enqueue(int thread_type, Evas_Thread_Command_Cb cb, void *data, int thread_mode)
{
   Evas_Thread *ev_thread = NULL;

   if (thread_type == EVAS_GL_THREAD_TYPE_GL)
      ev_thread = &evas_thread_gl;
   else if (thread_type == EVAS_GL_THREAD_TYPE_EVGL)
      ev_thread = &evas_thread_evgl;
   else
      ERR("GL thread type is invalid");

   if (thread_mode == EVAS_GL_THREAD_MODE_ENQUEUE)
      evas_thread_queue_append(ev_thread, cb, data, EINA_FALSE, EINA_FALSE);
   else if (thread_mode == EVAS_GL_THREAD_MODE_FLUSH)
      evas_thread_queue_append(ev_thread, cb, data, EINA_TRUE, EINA_FALSE);
   else if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     {
        evas_thread_queue_append(ev_thread, cb, data, EINA_TRUE, EINA_TRUE);
        while (ev_thread->waiting_cmd)
          {
             sched_yield();
          }
     }
   else
      ERR("GL thread mode is invalid");
}

EAPI Eina_Thread
evas_gl_thread_get(int thread_type)
{
   Evas_Thread *ev_thread = NULL;

   if (thread_type == EVAS_GL_THREAD_TYPE_GL)
      ev_thread = &evas_thread_gl;
   else if (thread_type == EVAS_GL_THREAD_TYPE_EVGL)
      ev_thread = &evas_thread_evgl;
   else
     {
        ERR("GL thread type is invalid");
        return (Eina_Thread)NULL;
     }

   return ev_thread->worker;
}


static void*
evas_thread_worker_func(void *data, Eina_Thread thread EINA_UNUSED)
{
   Evas_Thread *ev_thread = data;
   eina_thread_name_set(eina_thread_self(), ev_thread->thread_name);
   while (1)
     {
        Evas_Thread_Command *cmd;
        unsigned int len, max;

        eina_lock_take(&ev_thread->queue_lock);

        while (!ev_thread->queue_ready)
          {
             if (ev_thread->exit_thread)
               {
                  eina_lock_release(&ev_thread->queue_lock);
                  goto out;
               }
             eina_condition_wait(&ev_thread->queue_condition);
          }

        if (!eina_inarray_count(&ev_thread->queue))
          {
             ERR("Signaled to find an empty queue. BUG!");
             ev_thread->queue_ready = EINA_FALSE;
             eina_lock_release(&ev_thread->queue_lock);
             continue;
          }

        cmd = ev_thread->queue.members;
        ev_thread->queue.members = ev_thread->queue_cache;
        ev_thread->queue_cache = cmd;

        max = ev_thread->queue.max;
        ev_thread->queue.max = ev_thread->queue_cache_max;
        ev_thread->queue_cache_max = max;

        len = ev_thread->queue.len;
        ev_thread->queue.len = 0;

        ev_thread->queue_ready = EINA_FALSE;

        eina_lock_release(&ev_thread->queue_lock);

        DBG("Evas render thread command queue length: %u", len);

        eina_evlog("+thread", NULL, 0.0, NULL);
        while (len)
          {
             assert(cmd->cb);

             eina_evlog("+thread_do", cmd->data, 0.0, NULL);
             cmd->cb(cmd->data);
             eina_evlog("-thread_do", cmd->data, 0.0, NULL);
             eina_spinlock_take(&ev_thread->finish_spinlock);
             if (cmd == ev_thread->waiting_cmd)
               ev_thread->waiting_cmd = NULL;
             eina_spinlock_release(&ev_thread->finish_spinlock);

             cmd++;
             len--;
          }
        eina_evlog("-thread", NULL, 0.0, NULL);
     }

out:
   /* WRN: add a memory barrier or use a lock if we add more code here */
   ev_thread->exited = 1;
   return NULL;
}

static int
evas_thread_init(Evas_Thread *ev_thread, char *thread_name)
{
   ev_thread->thread_name = thread_name;
   ev_thread->queue_ready = EINA_FALSE;
   ev_thread->queue_cache = NULL;
   ev_thread->queue_cache_max = 0;

   ev_thread->exited = 0;
   ev_thread->exit_thread = EINA_FALSE;


   eina_inarray_step_set(&ev_thread->queue, sizeof (Eina_Inarray), sizeof (Evas_Thread_Command), 128);

   if (!eina_lock_new(&ev_thread->queue_lock))
      {
         CRI("Could not create draw thread queue lock");
         goto fail_on_lock_creation;
      }
   if (!eina_condition_new(&ev_thread->queue_condition, &ev_thread->queue_lock))
      {
         CRI("Could not create draw thread queue condition");
         goto fail_on_cond_creation;
      }
   if (!eina_spinlock_new(&ev_thread->finish_spinlock))
      {
         CRI("Could not create draw thread finish lock");
         goto fail_on_spin_lock_creation;
      }
   if (!eina_thread_create(&ev_thread->worker, EINA_THREAD_NORMAL, 0,
                           evas_thread_worker_func, ev_thread))
      {
         CRI("Could not create draw thread");
         goto fail_on_thread_creation;
      }
   return 1;
fail_on_thread_creation:
   ev_thread->worker = 0;
   eina_condition_free(&ev_thread->queue_condition);
fail_on_spin_lock_creation:
   eina_spinlock_free(&ev_thread->finish_spinlock);
fail_on_cond_creation:
   eina_lock_free(&ev_thread->queue_lock);
fail_on_lock_creation:
   eina_threads_shutdown();
fail_on_eina_thread_init:
   ev_thread->exited = 1;
   ev_thread->exit_thread = EINA_TRUE;

   return 0;
}

static void
evas_thread_shutdown(Evas_Thread *ev_thread)
{
   double to = 0;
   eina_lock_take(&ev_thread->queue_lock);

   ev_thread->exit_thread = EINA_TRUE;
   eina_condition_signal(&ev_thread->queue_condition);

   eina_lock_release(&ev_thread->queue_lock);

   _shutdown_timeout(&to, SHUTDOWN_TIMEOUT_RESET, SHUTDOWN_TIMEOUT);
   while (!ev_thread->exited &&
          evas_async_events_process() != -1)
     {
        if(_shutdown_timeout(&to, SHUTDOWN_TIMEOUT_CHECK, SHUTDOWN_TIMEOUT))
          {
             CRI("Timeout shutdown thread. Skipping thread_join. Some resources could be leaked");
             goto timeout_shutdown;
          }
     }

   eina_thread_join(ev_thread->worker);
timeout_shutdown:
   eina_spinlock_free(&ev_thread->finish_spinlock);
   eina_lock_free(&ev_thread->queue_lock);
   eina_condition_free(&ev_thread->queue_condition);

   ev_thread->worker = 0;

   eina_inarray_flush(&ev_thread->queue);
   free(ev_thread->queue_cache);

}

int
evas_threads_init(void)
{
   if (evas_threads_init_count++) return evas_threads_init_count;

   if(!eina_threads_init())
     {
        CRI("Could not init eina threads");
        return --evas_threads_init_count;
     }
   if (!evas_thread_init(&evas_thread_software, "Eevas-thread-wk-sw")) return --evas_threads_init_count;
   if (!evas_thread_init(&evas_thread_gl, "Eevas-thread-wk-gl"))       return --evas_threads_init_count;
   if (!evas_thread_init(&evas_thread_evgl, "Eevas-thread-wk-evgl"))   return --evas_threads_init_count;

   return evas_threads_init_count;
}

void
evas_threads_shutdown(void)
{
   assert(evas_threads_init_count);

   if (--evas_threads_init_count)
     return;

   evas_thread_shutdown(&evas_thread_evgl);
   evas_thread_shutdown(&evas_thread_gl);
   evas_thread_shutdown(&evas_thread_software);
   eina_threads_shutdown();
}
