#include "evas_common_private.h"

#include <assert.h>

typedef struct
{
  const char *thread_name;
  Eina_Thread worker;
  Eina_Thread_Queue *thread_queue_command;
  Eina_Thread_Queue *thread_queue_finish;

  volatile int exited;
  Eina_Bool exit_thread;
} Evas_Thread;

typedef struct
{
  Eina_Thread_Queue_Msg thq_head;
  void *data;
} Evas_Thread_Finish_Reply;

static int evas_threads_sw_init_count = 0;
static int evas_threads_gl_init_count = 0;

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
evas_thread_queue_append(Evas_Thread *ev_thread, Evas_Thread_Command_Cb cb, void *data, Eina_Bool do_flush EINA_UNUSED, Eina_Bool need_finish, Eina_Bool do_finish)
{
  Evas_Thread_Command *cmd;
  void *ref;

  cmd = eina_thread_queue_send(ev_thread->thread_queue_command, sizeof(Evas_Thread_Command), &ref);


  if (cmd)
    {
      cmd->cb = cb;
      cmd->data = data;

        if (need_finish)
        cmd->finish = EINA_TRUE;
      else
        cmd->finish = EINA_FALSE;

      eina_thread_queue_send_done(ev_thread->thread_queue_command, ref);
    }
  else
    {
      ERR("Out of memory allocating thread command.");
      return;
    }

  if (do_finish)
    {
      while(EINA_TRUE)
        {
          void *finish_ref;
          Evas_Thread_Finish_Reply *rep;

          rep = eina_thread_queue_wait(ev_thread->thread_queue_finish, &finish_ref);
          if (rep && rep->data == cmd->data)
            {
              eina_thread_queue_wait_done(ev_thread->thread_queue_finish, finish_ref);
              break;
            }
          eina_thread_queue_wait_done(ev_thread->thread_queue_finish, finish_ref);
        }
    }
}

EAPI void
evas_thread_cmd_enqueue(Evas_Thread_Command_Cb cb, void *data)
{
   evas_thread_queue_append(&evas_thread_software, cb, data, EINA_FALSE, EINA_FALSE, EINA_FALSE);
}

EAPI void
evas_thread_queue_flush(Evas_Thread_Command_Cb cb, void *data)
{
   evas_thread_queue_append(&evas_thread_software, cb, data, EINA_TRUE, EINA_FALSE, EINA_FALSE);
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
    {
      ERR("GL thread type is invalid");
      goto out;
    }

  if (thread_mode == EVAS_GL_THREAD_MODE_ENQUEUE)
     evas_thread_queue_append(ev_thread, cb, data, EINA_FALSE, EINA_FALSE, EINA_FALSE);
  else if (thread_mode == EVAS_GL_THREAD_MODE_FLUSH)
     evas_thread_queue_append(ev_thread, cb, data, EINA_TRUE, EINA_FALSE, EINA_FALSE);
  else if (thread_mode == EVAS_GL_THREAD_MODE_FINISH)
     evas_thread_queue_append(ev_thread, cb, data, EINA_TRUE, EINA_TRUE, EINA_TRUE);
  else if (thread_mode == EVAS_GL_THREAD_MODE_ASYNC_FINISH)
     evas_thread_queue_append(ev_thread, cb, data, EINA_TRUE, EINA_TRUE, EINA_FALSE);
  else
    {
      ERR("GL thread mode is invalid");
      goto out;
    }

out:
  return;
}

EAPI void
evas_gl_thread_cmd_wait(int thread_type, void *data, Eina_Bool *finished_ptr)
{
   Evas_Thread *ev_thread = NULL;

   if (thread_type == EVAS_GL_THREAD_TYPE_GL)
      ev_thread = &evas_thread_gl;
   else if (thread_type == EVAS_GL_THREAD_TYPE_EVGL)
      ev_thread = &evas_thread_evgl;
   else
     {
        ERR("GL thread type is invalid");
        goto out;
     }

   while (!(*finished_ptr))
     {
        void *finish_ref;
        Evas_Thread_Finish_Reply *rep;

        rep = eina_thread_queue_wait(ev_thread->thread_queue_finish, &finish_ref);
        if (rep && rep->data == data)
          {
             eina_thread_queue_wait_done(ev_thread->thread_queue_finish, finish_ref);
             return;


          }
        eina_thread_queue_wait_done(ev_thread->thread_queue_finish, finish_ref);
     }

out:
     return;
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
      goto out;
    }

  return ev_thread->worker;

out:
  return (Eina_Thread)NULL;
}

static void*
evas_thread_worker_func(void *data, Eina_Thread thread EINA_UNUSED)
{
  Evas_Thread *ev_thread = data;
  void *ref;

  eina_thread_name_set(eina_thread_self(), ev_thread->thread_name);

  while (1)
    {
      Evas_Thread_Command *cmd;

      cmd = eina_thread_queue_wait(ev_thread->thread_queue_command, &ref);

      if (cmd && cmd->cb)
        {
          eina_evlog("+thread_do", cmd->data, 0.0, NULL);
          cmd->cb(cmd->data);
          eina_evlog("-thread_do", cmd->data, 0.0, NULL);

          if (cmd->finish)
            {
              void *finish_ref;
              Evas_Thread_Finish_Reply *rep;

              rep = eina_thread_queue_send(ev_thread->thread_queue_finish,
                                           sizeof(Evas_Thread_Finish_Reply),
                                           &finish_ref);
              rep->data = cmd->data;
              eina_thread_queue_send_done(ev_thread->thread_queue_finish, finish_ref);
            }

        }
      else
        {
          eina_thread_queue_wait_done(ev_thread->thread_queue_command, ref);
          goto out;
        }
      eina_thread_queue_wait_done(ev_thread->thread_queue_command, ref);
    }

out:
  ev_thread->exited = 1;

  return NULL;
}

static Eina_Bool
evas_thread_init(Evas_Thread *ev_thread, const char *thread_name)
{
  ev_thread->thread_name = thread_name;

  ev_thread->thread_queue_command = eina_thread_queue_new();
  if (!ev_thread->thread_queue_command)
    {
      CRI("Could not create thread queue for command (%m)");
      goto fail_on_thread_queue_command_creation;
    }

  ev_thread->thread_queue_finish = eina_thread_queue_new();
  if (!ev_thread->thread_queue_finish)
    {
      CRI("Could not create thread queue for finish (%m)");
      goto fail_on_thread_queue_finish_creation;
    }

  ev_thread->exit_thread = EINA_FALSE;
  ev_thread->exited = 0;

  if (!eina_thread_create(&ev_thread->worker, EINA_THREAD_NORMAL, -1,
                          evas_thread_worker_func, ev_thread))
    {
      CRI("Could not create draw thread (%m)");
      goto fail_on_thread_creation;
    }

  return EINA_TRUE;

fail_on_thread_creation:
  ev_thread->worker = 0;
  eina_thread_queue_free(ev_thread->thread_queue_finish);
fail_on_thread_queue_finish_creation:
  eina_thread_queue_free(ev_thread->thread_queue_command);
fail_on_thread_queue_command_creation:
  ev_thread->exit_thread = EINA_TRUE;
  ev_thread->exited = 1;
  return EINA_FALSE;
}
static void
evas_thread_shutdown(Evas_Thread *ev_thread)
{
  Evas_Thread_Command *cmd;
  void *ref;
  double to = 0;

  if (!ev_thread) return;

  ev_thread->exit_thread = EINA_TRUE;

  cmd = eina_thread_queue_send(ev_thread->thread_queue_command, sizeof(Evas_Thread_Command), &ref);
  if (cmd)
    {
      cmd->cb = NULL;
      cmd->data = NULL;
      cmd->finish = EINA_FALSE;
      eina_thread_queue_send_done(ev_thread->thread_queue_command, ref);
    }

  _shutdown_timeout(&to, SHUTDOWN_TIMEOUT_RESET, SHUTDOWN_TIMEOUT);
  while (!ev_thread->exited && (evas_async_events_process() != -1))
    {
      if(_shutdown_timeout(&to, SHUTDOWN_TIMEOUT_CHECK, SHUTDOWN_TIMEOUT))
        {
          CRI("Timeout shutdown thread. Skipping thread_join. Some resources could be leaked");
          goto timeout_shutdown;
        }
    }

  eina_thread_join(ev_thread->worker);

timeout_shutdown:
  eina_thread_queue_free(ev_thread->thread_queue_command);
  eina_thread_queue_free(ev_thread->thread_queue_finish);
  ev_thread->worker = 0;
}

EAPI int
evas_threads_sw_init(void)
{
  if (evas_threads_sw_init_count++)
    return evas_threads_sw_init_count;

  if (!eina_threads_init())
    {
      CRI("Could not init eina threads");
      goto fail_on_eina_thread_init;
    }

  if (!evas_thread_init(&evas_thread_software, "Evas-thread-wk-sw"))
    goto fail_on_software_thread_init;
  return evas_threads_sw_init_count;

fail_on_software_thread_init:
  eina_threads_shutdown();
fail_on_eina_thread_init:
  return --evas_threads_sw_init_count;
}

EAPI int
evas_threads_gl_init(void)
{
  if (evas_threads_gl_init_count++)
    return evas_threads_gl_init_count;

  if (!eina_threads_init())
    {
      CRI("Could not init eina threads");
      goto fail_on_eina_thread_init;
    }

  if (!evas_thread_init(&evas_thread_gl, "Evas-thread-wk-gl"))
    goto fail_on_gl_thread_init;
  if (!evas_thread_init(&evas_thread_evgl, "Evas-thread-wk-evgl"))
    goto fail_on_evgl_thread_init;
  return evas_threads_gl_init_count;

fail_on_evgl_thread_init:
  evas_thread_shutdown(&evas_thread_gl);
fail_on_gl_thread_init:
  eina_threads_shutdown();
fail_on_eina_thread_init:
  return --evas_threads_gl_init_count;
}

EAPI int
evas_threads_sw_shutdown(void)
{
  if (evas_threads_sw_init_count <= 0)
    {
      ERR("Too many calls to shutdown, ignored.");
      return 0;
    }

  if (--evas_threads_sw_init_count)
    return evas_threads_sw_init_count;

  evas_thread_shutdown(&evas_thread_software);

  eina_threads_shutdown();

  return 0;
}

EAPI int
evas_threads_gl_shutdown(void)
{
  if (evas_threads_gl_init_count <= 0)
    {
      ERR("Too many calls to shutdown, ignored.");
      return 0;
    }

  if (--evas_threads_gl_init_count)
    return evas_threads_gl_init_count;

  evas_thread_shutdown(&evas_thread_evgl);
  evas_thread_shutdown(&evas_thread_gl);


  eina_threads_shutdown();

  return 0;
}
