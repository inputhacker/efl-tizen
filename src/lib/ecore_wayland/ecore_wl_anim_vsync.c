#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include "Ecore.h"

#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <tdm_client.h>
#include <poll.h>
#include <errno.h>

static tdm_client *client = NULL;
static tdm_client_output *output = NULL;
static tdm_client_vblank *vblank = NULL;
static Eina_Bool vblank_wait= 0;
static int _vsync_log_dom = -1;
static int tdm_fd = -1;
static double _tdm_req_fps = 1.0 / 60.0;
static Eina_Thread_Queue *thq = NULL;
static Ecore_Thread *tdm_thread = NULL;
static Eina_Bool tdm_event_is_busy = EINA_FALSE;
static Eina_Bool tick_skip = EINA_FALSE;
static Eina_Spinlock tick_queue_lock;
static int           tick_queue_count = 0;
typedef struct
{
   Eina_Thread_Queue_Msg head;
   char val;
} Msg;

#define DELTA_COUNT 10
#undef ERR
#define ERR(...) EINA_LOG_DOM_ERR(_vsync_log_dom, __VA_ARGS__)

#undef DBG
#define DBG(...) EINA_LOG_DOM_DBG(_vsync_log_dom, __VA_ARGS__)

#undef INF
#define INF(...) EINA_LOG_DOM_INFO(_vsync_log_dom, __VA_ARGS__)

#undef WRN
#define WRN(...) EINA_LOG_DOM_WARN(_vsync_log_dom, __VA_ARGS__)

#undef CRI
#define CRI(...) EINA_LOG_DOM_CRIT(_vsync_log_dom, __VA_ARGS__)


static void
_tick_send(char val)
{
   Msg *msg;
   void *ref;

   if (!thq) return;
   msg = eina_thread_queue_send(thq, sizeof(Msg), &ref);
   msg->val = val;
   eina_thread_queue_send_done(thq, ref);
}

static void
_tdm_send_time(double t)
{
   double *tim = malloc(sizeof(*tim));
   if (tim)
     {
        *tim = t;
        DBG("tdm send time  @%1.5f   ... send %1.8f\n", ecore_time_get(), t);
        eina_spinlock_take(&tick_queue_lock);
        tick_queue_count++;
        eina_spinlock_release(&tick_queue_lock);
        ecore_thread_feedback(tdm_thread, tim);
     }
}

static void
_tdm_vblank_handler(tdm_client_vblank *vblank EINA_UNUSED, tdm_error error EINA_UNUSED, unsigned int sequence,
                    unsigned int tv_sec, unsigned int tv_usec, void *user_data EINA_UNUSED)
{
   vblank_wait = 0;

   if (tdm_event_is_busy)
     {
        static unsigned int pseq = 0;
        if (pseq != sequence)
          {
             double t = (double)tv_sec + ((double)tv_usec / 1000000);
             double tnow = ecore_time_get();
             static double tdelta[DELTA_COUNT];
             static double tdelta_avg = 0.0;
             static int tdelta_n = 0;

             if (t > tnow)
               {
                  if (tdelta_n > DELTA_COUNT)
                    {
                       t = t + tdelta_avg;
                    }
                  else if (tdelta_n < DELTA_COUNT)
                    {
                       tdelta[tdelta_n] = tnow - t;
                       tdelta_n++;
                       t = tnow;
                    }
                  else if (tdelta_n == DELTA_COUNT)
                    {
                       int i;
                       for (i = 0; i < DELTA_COUNT; i++)
                         tdelta_avg += tdelta[i];
                       tdelta_avg /= (double)(DELTA_COUNT);
                       tdelta_n++;
                    }
               }
             else
               {
                  tdelta_avg = 0.0;
                  tdelta_n = 0;
               }
             _tdm_send_time(t);
             pseq = sequence;
          }
     }
}

static tdm_error
_tdm_tick_schedule(void)
{
   tdm_error err;
   err = tdm_client_vblank_wait(vblank, 1, _tdm_vblank_handler, NULL);
   if (err != TDM_ERROR_NONE)
     ERR("tdm client vblank wait failed %d\n", err);
   else vblank_wait = 1;
   return err;
}

static void
_tdm_tick_core(void *data EINA_UNUSED, Ecore_Thread *thread)
{
   Msg *msg;
   void *ref;
   int tick = 0;
   double ptime = 0.0;
   tdm_error err;
   eina_thread_name_set(eina_thread_self(), "Eanimator-vsync");
   struct pollfd fds;

   fds.events = POLLIN;
   fds.fd = tdm_fd;
   fds.revents = 0;
   while (!ecore_thread_check(thread))
     {
        DBG("------- tdm_event_is_busy=%i", tdm_event_is_busy);
        if (!tdm_event_is_busy)
          {
             DBG("wait...");
             msg = eina_thread_queue_wait(thq, &ref);
             if (msg)
               {
                  tick = msg->val;
                  eina_thread_queue_wait_done(thq, ref);
               }
          }
        else
          {
             do
               {
                  DBG("poll...");
                  msg = eina_thread_queue_poll(thq, &ref);
                  if (msg)
                    {
                       tick = msg->val;
                       eina_thread_queue_wait_done(thq, ref);
                    }
               }
             while (msg);
          }
        if (tick == -1)
          goto done;
        else if (tick == 1)
          {
             int ret;
             if (!vblank_wait)
               {
                  if(_tdm_tick_schedule() != TDM_ERROR_NONE)
                    goto done;
                  ptime = ecore_time_get();
               }
             else if ((ptime + _tdm_req_fps) <= ecore_time_get())
               DBG("tdm vblank handler does not called in %lfms\n", ecore_time_get() - ptime);

             ret = poll(&fds, 1, -1);

             if (ret == 1)
               {
                  err = tdm_client_handle_events(client);
                  if (err != TDM_ERROR_NONE) {
                       ERR("tdm_client_handle_events failed err %d\n", err);
                       vblank_wait = 0;
                       _tdm_send_time(ecore_time_get());
                  }
               }
             else
               {
                  if ((ret < 0) && (errno != EINTR) && (errno != EAGAIN))  /* normal case */
                    {
                       ERR("tdm_fd poll fail\n");
                       goto done;
                    }
               }
          }
     }

done:
   if (vblank)
     tdm_client_vblank_destroy(vblank);
   if (client)
     tdm_client_destroy(client);
   tdm_fd = -1;
   vblank_wait = 0;
   vblank = client = output = NULL;

   return;

}

static void
_tdm_tick_notify(void *data EINA_UNUSED, Ecore_Thread *thread EINA_UNUSED, void *msg)
{
   int tick_queued;

   eina_spinlock_take(&tick_queue_lock);
   tick_queued = tick_queue_count;
   tick_queue_count--;
   eina_spinlock_release(&tick_queue_lock);
   DBG("notify.... %3.3f %i", *((double *)msg), tdm_event_is_busy);

   if (tdm_event_is_busy)
     {
        double *t = msg;
        static double pt = 0.0;

        DBG("VSYNC %1.8f = delt %1.8f", *t, *t - pt);
        if (((!tick_skip) || (tick_skip && tick_queued == 1)) && (*t > pt))
          {
             ecore_loop_time_set(*t);
             ecore_animator_custom_tick();
             pt = *t;
          }
        else if ((tick_skip) && (tick_queued > 10))
          {
             DBG("skip this vsync for schedule queued %d\n", tick_queued);
          }
     }
   free(msg);
}

static Eina_Bool
_tdm_client_init(void)
{
   tdm_error error;
   double fps;

   if (!client)
     {
        client = tdm_client_create(&error);
        if (error != TDM_ERROR_NONE) {
             ERR("tdm_client_create error %d\n", error);
             goto done;
        }

        error = tdm_client_get_fd(client, &tdm_fd);
        if (error != TDM_ERROR_NONE) {
             ERR("tdm_client_get_fd error %d\n", error);
             goto done;
        }

        output = tdm_client_get_output(client, NULL, &error);
        if (error != TDM_ERROR_NONE)
          {
             ERR("tdm_client_get_output error %d\n", error);
             goto done;
          }


        vblank = tdm_client_output_create_vblank(output, &error);
        if (error != TDM_ERROR_NONE) {
             ERR("tdm_client_output_create_vblank error %d\n", error);
             goto done;
        }

        tdm_client_vblank_set_enable_fake(vblank, 1);
     }

   fps = ecore_animator_frametime_get();
   if (_tdm_req_fps != fps)
     {
        _tdm_req_fps = fps;
        tdm_client_vblank_set_fps(vblank, (unsigned int)(1 / fps));
     }

   return EINA_TRUE;

done:
   if (vblank) tdm_client_vblank_destroy(vblank);
   if (client) tdm_client_destroy(client);
   vblank = client = output = NULL;
   tdm_fd = -1;
   return EINA_FALSE;
}

static void
_tdm_tick_finished(void *data EINA_UNUSED, Ecore_Thread *thread EINA_UNUSED)
{

   if (vblank)
     tdm_client_vblank_destroy(vblank);

   if (client)
     tdm_client_destroy(client);

   if (_vsync_log_dom > 0)
        eina_log_domain_unregister(_vsync_log_dom);

   _vsync_log_dom = -1;
   tdm_fd = -1;
   vblank_wait = 0;
   vblank = client = output = NULL;

   eina_spinlock_free(&tick_queue_lock);
   eina_thread_queue_free(thq);
   tick_queue_count = 0;
   tdm_thread = NULL;
   thq = NULL;
}

void
_ecore_wl_animator_vsync_tick_quit(EINA_UNUSED void *data)
{
   int i;

   if (!thq) return;

   _tick_send(-1);

   for (i = 0; (i < 500) && (tdm_thread); i++)
     {
        usleep(1000);
     }
}

void
_ecore_wl_animator_vsync_tick_begin(EINA_UNUSED void *data)
{
   if (_vsync_log_dom < 0)
     _vsync_log_dom = eina_log_domain_register("ecore_anim_vsync", EINA_COLOR_LIGHTRED);

   if (_tdm_client_init())
     {
        if (getenv("ECORE_ANIMATOR_SKIP")) tick_skip = EINA_TRUE;
        tdm_event_is_busy = 1;
        if (!tdm_thread)
          {
             eina_spinlock_new(&tick_queue_lock);
             thq = eina_thread_queue_new();
             tdm_thread = ecore_thread_feedback_run(_tdm_tick_core,
                                                    _tdm_tick_notify,
                                                    _tdm_tick_finished,
                                                    _tdm_tick_finished,
                                                    NULL, EINA_TRUE);
             tick_queue_count = 0;
          }
        _tick_send(1);
     }
   else
     ERR("tdm tick begin failed\n");
}

void
_ecore_wl_animator_vsync_tick_end(EINA_UNUSED void *data)
{
   if (!thq) return;
   tdm_event_is_busy = 0;
   _tick_send(0);
}

