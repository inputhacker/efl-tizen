#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <pthread.h>

#include "Ecore.h"
#include "ecore_private.h"

/* make mono happy - this is evil though... */
#undef SIGPWR

#define ECORE_SIGNAL_THREAD 1

static void _ecore_signal_exe_exit_delay(void *data, const Efl_Event *event);
static void _ecore_signal_waitpid(Eina_Bool once, siginfo_t info);
static void _ecore_signal_generic_free(void *data, void *event);

typedef void (*Signal_Handler)(int sig, siginfo_t *si, void *foo);

#ifdef ECORE_SIGNAL_THREAD
static Eina_Thread sig_thread;
static Eina_Bool sig_thread_exists = EINA_FALSE;
#endif
static int sig_pipe[2] = { -1, -1 }; // [0] == read, [1] == write
static Eo *sig_pipe_handler = NULL;

typedef struct _Signal_Data
{
   int sig;
   siginfo_t info;
} Signal_Data;

static Eina_Bool
_ecore_signal_pipe_read(Eo *obj)
{
   Signal_Data sdata;
   int ret;

   ret = read(sig_pipe[0], &sdata, sizeof(sdata));
   if (ret != sizeof(sdata)) return EINA_FALSE;
   switch (sdata.sig)
     {
      case SIGPIPE:
        break;
      case SIGALRM:
        break;
      case SIGCHLD:
        _ecore_signal_waitpid(EINA_FALSE, sdata.info);
        break;
      case SIGUSR1:
      case SIGUSR2:
          {
             Ecore_Event_Signal_User *e = _ecore_event_signal_user_new();
             if (e)
               {
                  if (sdata.sig == SIGUSR1) e->number = 1;
                  else e->number = 2;
                  e->data = sdata.info;
                  ecore_event_add(ECORE_EVENT_SIGNAL_USER, e,
                                  _ecore_signal_generic_free, NULL);
               }
             Eo *loop = efl_provider_find(obj, EFL_LOOP_CLASS);
             if (loop)
               {
                  if (sdata.sig == SIGUSR1)
                    efl_event_callback_call(loop, EFL_LOOP_EVENT_SIGNAL_USR1, NULL);
                  else
                    efl_event_callback_call(loop, EFL_LOOP_EVENT_SIGNAL_USR2, NULL);
               }
          }
        break;
      case SIGHUP:
          {
             Ecore_Event_Signal_Hup *e = _ecore_event_signal_hup_new();
             if (e)
               {
                  e->data = sdata.info;
                  ecore_event_add(ECORE_EVENT_SIGNAL_HUP, e,
                                  _ecore_signal_generic_free, NULL);
               }
             Eo *loop = efl_provider_find(obj, EFL_LOOP_CLASS);
             if (loop)
               efl_event_callback_call(loop, EFL_LOOP_EVENT_SIGNAL_HUP, NULL);
          }
        break;
      case SIGQUIT:
      case SIGINT:
      case SIGTERM:
          {
             Ecore_Event_Signal_Exit *e = _ecore_event_signal_exit_new();
             if (e)
               {
                  if (sdata.sig == SIGQUIT) e->quit = 1;
                  else if (sdata.sig == SIGINT) e->interrupt = 1;
                  else e->terminate = 1;
                  e->data = sdata.info;
                  ecore_event_add(ECORE_EVENT_SIGNAL_EXIT, e,
                                  _ecore_signal_generic_free, NULL);
               }
          }
        break;
#ifdef SIGPWR
      case SIGPWR:
          {
             Ecore_Event_Signal_Power *e = _ecore_event_signal_power_new();
             if (e)
               {
                  e->data = sdata.info;
                  ecore_event_add(ECORE_EVENT_SIGNAL_POWER, e,
                                  _ecore_signal_generic_free, NULL);
               }
          }
        break;
#endif
      default:
        break;
     }
   return EINA_TRUE;
}

static void
_ecore_signal_cb_read(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   while (_ecore_signal_pipe_read(event->object));
}

static void
_ecore_signal_cb_del(void *data EINA_UNUSED, const Efl_Event *event)
{
   if (event->object == sig_pipe_handler) sig_pipe_handler = NULL;
}

EFL_CALLBACKS_ARRAY_DEFINE(_event_watch,
                             { EFL_LOOP_HANDLER_EVENT_READ, _ecore_signal_cb_read },
                             { EFL_EVENT_DEL, _ecore_signal_cb_del });

static void
_ecore_signal_callback(int sig, siginfo_t *si, void *foo EINA_UNUSED)
{
   Signal_Data sdata;

   sdata.sig = sig;
   sdata.info = *si;
   if (sdata.sig >= 0) write(sig_pipe[1], &sdata, sizeof(sdata));
}

static void
_ecore_signal_callback_set(int sig, Signal_Handler func)
{
   struct sigaction sa;

#ifdef ECORE_SIGNAL_THREAD
   if (eina_thread_self() != sig_thread)
     {
        fprintf(stderr, "Ecore sig handler NOT called from sigwatcher thread\n");
     }
#endif
   sa.sa_sigaction = func;
   sa.sa_flags = SA_RESTART | SA_SIGINFO;
   sigemptyset(&sa.sa_mask);
   sigaction(sig, &sa, NULL);
}

static void
_signalhandler_setup(void)
{
   sigset_t newset;

   _ecore_signal_callback_set(SIGPIPE, _ecore_signal_callback);
   _ecore_signal_callback_set(SIGALRM, _ecore_signal_callback);
   _ecore_signal_callback_set(SIGCHLD, _ecore_signal_callback);
   _ecore_signal_callback_set(SIGUSR1, _ecore_signal_callback);
   _ecore_signal_callback_set(SIGUSR2, _ecore_signal_callback);
   _ecore_signal_callback_set(SIGHUP,  _ecore_signal_callback);
   _ecore_signal_callback_set(SIGQUIT, _ecore_signal_callback);
   _ecore_signal_callback_set(SIGINT,  _ecore_signal_callback);
   _ecore_signal_callback_set(SIGTERM, _ecore_signal_callback);
#ifdef SIGPWR
   _ecore_signal_callback_set(SIGPWR,  _ecore_signal_callback);
#endif

#ifndef _WIN32
   sigemptyset(&newset);
   sigaddset(&newset, SIGPIPE);
   sigaddset(&newset, SIGALRM);
   sigaddset(&newset, SIGCHLD);
   sigaddset(&newset, SIGUSR1);
   sigaddset(&newset, SIGUSR2);
   sigaddset(&newset, SIGHUP);
   sigaddset(&newset, SIGQUIT);
   sigaddset(&newset, SIGINT);
   sigaddset(&newset, SIGTERM);
# ifdef SIGPWR
   sigaddset(&newset, SIGPWR);
# endif
   pthread_sigmask(SIG_UNBLOCK, &newset, NULL);
#endif
}

static void *
_ecore_signal_thread_watcher(void *data EINA_UNUSED, Eina_Thread t)
{
   eina_thread_cancellable_set(EINA_FALSE, NULL);
   eina_thread_name_set(t, "Esigwatcher");
   _signalhandler_setup();
   for (;;) pause();
   return NULL;
}

static void
_ecore_signal_pipe_init(void)
{
   if (sig_pipe[0] == -1)
     {
        if (pipe(sig_pipe) != 0)
          {
             sig_pipe[0] = -1;
             return;
          }
        eina_file_close_on_exec(sig_pipe[0], EINA_TRUE);
        eina_file_close_on_exec(sig_pipe[1], EINA_TRUE);
        fcntl(sig_pipe[0], F_SETFL, O_NONBLOCK);
     }
#ifdef ECORE_SIGNAL_THREAD
   if (!sig_thread_exists)
     {
        if (!eina_thread_create(&sig_thread, EINA_THREAD_NORMAL,
                                -1, _ecore_signal_thread_watcher, NULL))
          {
             close(sig_pipe[0]);
             close(sig_pipe[1]);
             sig_pipe[0] = -1;
             sig_pipe[1] = -1;
             return;
          }
        sig_thread_exists = EINA_TRUE;
     }
#else
   _signalhandler_setup();
#endif
   if (!sig_pipe_handler)
     sig_pipe_handler =
       efl_add(EFL_LOOP_HANDLER_CLASS, ML_OBJ,
               efl_loop_handler_fd_set(efl_added, sig_pipe[0]),
               efl_loop_handler_active_set(efl_added, EFL_LOOP_HANDLER_FLAGS_READ),
               efl_event_callback_array_add(efl_added, _event_watch(), NULL));
}

static void
_ecore_signal_pipe_shutdown(void)
{
   if (sig_pipe[0] != -1)
     {
        close(sig_pipe[0]);
        close(sig_pipe[1]);
        sig_pipe[0] = -1;
        sig_pipe[1] = -1;
     }
   if (sig_pipe_handler)
     {
        efl_del(sig_pipe_handler);
        sig_pipe_handler = NULL;
     }
}

static void
_ecore_signal_cb_fork(void *data EINA_UNUSED)
{
   _ecore_signal_pipe_shutdown();
#ifdef ECORE_SIGNAL_THREAD
   sig_thread_exists = EINA_FALSE;
#endif
   _ecore_signal_pipe_init();
}

void
_ecore_signal_init(void)
{
#ifndef _WIN32
   sigset_t newset;

   sigemptyset(&newset);
   sigaddset(&newset, SIGPIPE);
   sigaddset(&newset, SIGALRM);
   sigaddset(&newset, SIGCHLD);
   sigaddset(&newset, SIGUSR1);
   sigaddset(&newset, SIGUSR2);
   sigaddset(&newset, SIGHUP);
   sigaddset(&newset, SIGQUIT);
   sigaddset(&newset, SIGINT);
   sigaddset(&newset, SIGTERM);
# ifdef SIGPWR
   sigaddset(&newset, SIGPWR);
# endif
   pthread_sigmask(SIG_BLOCK, &newset, NULL);
#endif
   _ecore_signal_pipe_init();
   ecore_fork_reset_callback_add(_ecore_signal_cb_fork, NULL);
}

void
_ecore_signal_shutdown(void)
{
   ecore_fork_reset_callback_del(_ecore_signal_cb_fork, NULL);
   _ecore_signal_pipe_shutdown();
   // we probably should restore.. but not a good idea
   // pthread_sigmask(SIG_SETMASK, &sig_oldset, NULL);
}

void
_ecore_signal_received_process(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd EINA_UNUSED)
{
   // do nothing - the efl loop handler read event will handle it
}

int
_ecore_signal_count_get(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd EINA_UNUSED)
{
   // we will always have 0 signals be3cause they will instead be read from
   // a pipe fd and placed in a queue/list that
   // _ecore_signal_received_process() will then walk and process/do
   return 0;
}

static void
_ecore_signal_exe_exit_delay(void *data, const Efl_Event *event)
{
   Ecore_Exe_Event_Del *e = data;

   if (!e) return;
   _ecore_exe_doomsday_clock_set(e->exe, NULL);
   ecore_event_add(ECORE_EXE_EVENT_DEL, e,
                   _ecore_exe_event_del_free, NULL);
   efl_del(event->object);
}

static void
_ecore_signal_waitpid(Eina_Bool once, siginfo_t info)
{
   pid_t pid;
   int status;

   while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
     {
        Ecore_Exe_Event_Del *e = _ecore_exe_event_del_new();

        //FIXME: If this process is set respawn, respawn with a suitable backoff
        // period for those that need too much respawning.
        if (e)
          {
             if (WIFEXITED(status))
               {
                  e->exit_code = WEXITSTATUS(status);
                  e->exited = 1;
               }
             else if (WIFSIGNALED(status))
               {
                  e->exit_signal = WTERMSIG(status);
                  e->signalled = 1;
               }
             e->pid = pid;
             e->exe = _ecore_exe_find(pid);
             e->data = info;  // No need to clone this.
             if ((e->exe) &&
                 (ecore_exe_flags_get(e->exe) &
                  (ECORE_EXE_PIPE_READ | ECORE_EXE_PIPE_ERROR)))
               {
                  /* We want to report the Last Words of the exe, so delay this event.
                   * This is twice as relevant for stderr.
                   * There are three possibilities here -
                   *  1 There are no Last Words.
                   *  2 There are Last Words, they are not ready to be read.
                   *  3 There are Last Words, they are ready to be read.
                   *
                   * For 1 we don't want to delay, for 3 we want to delay.
                   * 2 is the problem.  If we check for data now and there
                   * is none, then there is no way to differentiate 1 and 2.
                   * If we don't delay, we may loose data, but if we do delay,
                   * there may not be data and the exit event never gets sent.
                   *
                   * Any way you look at it, there has to be some time passed
                   * before the exit event gets sent.  So the strategy here is
                   * to setup a timer event that will send the exit event after
                   * an arbitrary, but brief, time.
                   *
                   * This is probably paranoid, for the less paraniod, we could
                   * check to see for Last Words, and only delay if there are any.
                   * This has it's own set of problems. */
                  efl_del(_ecore_exe_doomsday_clock_get(e->exe));

                  Efl_Loop_Timer *doomsday_clock =
                    efl_add(EFL_LOOP_TIMER_CLASS, ML_OBJ,
                            efl_loop_timer_interval_set(efl_added, 0.1),
                            efl_event_callback_add
                            (efl_added, EFL_LOOP_TIMER_EVENT_TICK,
                             _ecore_signal_exe_exit_delay, e));
                  _ecore_exe_doomsday_clock_set(e->exe, doomsday_clock);
               }
             else ecore_event_add(ECORE_EXE_EVENT_DEL, e,
                                  _ecore_exe_event_del_free, NULL);
          }
        if (once) break;
     }
}

static void
_ecore_signal_generic_free(void *data EINA_UNUSED, void *event)
{
   free(event);
}
