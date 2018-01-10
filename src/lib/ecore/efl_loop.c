#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "Ecore.h"
#include "ecore_private.h"

#include "ecore_main_common.h"

typedef struct _Efl_Loop_Promise_Simple_Data Efl_Loop_Promise_Simple_Data;
typedef struct _Efl_Internal_Promise Efl_Internal_Promise;

struct _Efl_Loop_Promise_Simple_Data
{
   union {
      Efl_Loop_Timer *timer;
      Ecore_Idler *idler;
   };
   Eina_Promise *promise;
};
GENERIC_ALLOC_SIZE_DECLARE(Efl_Loop_Promise_Simple_Data);

EOLIAN static Efl_Loop_Message_Handler *
_efl_loop_message_handler_get(Eo *obj EINA_UNUSED, void *pd EINA_UNUSED, Efl_Loop *loop, const Efl_Class *klass)
{
   Message_Handler mh = { 0 }, *mh2;
   Efl_Loop_Data *ld = efl_data_scope_get(loop, EFL_LOOP_CLASS);
   unsigned int i, n;

   if (!ld) return NULL;
   n = eina_inarray_count(ld->message_handlers);
   for (i = 0; i < n; i++)
     {
        mh2 = eina_inarray_nth(ld->message_handlers, i);
        if (mh2->klass == klass) return mh2->handler;
     }
   mh.klass = klass;
   mh.handler = efl_add(klass, loop);
   eina_inarray_push(ld->message_handlers, &mh);
   return mh.handler;
}

Efl_Version _app_efl_version = { 0, 0, 0, 0, NULL, NULL };

Eo            *_mainloop_singleton = NULL;
Efl_Loop_Data *_mainloop_singleton_data = NULL;

EOLIAN static Efl_Loop *
_efl_loop_main_get(Efl_Class *klass EINA_UNUSED, void *_pd EINA_UNUSED)
{
   if (_mainloop_singleton) return _mainloop_singleton;
   _mainloop_singleton = efl_add(EFL_LOOP_CLASS, NULL);
   _mainloop_singleton_data = efl_data_scope_get(_mainloop_singleton, EFL_LOOP_CLASS);
   return _mainloop_singleton;
}

EAPI Eo *
ecore_main_loop_get(void)
{
   return efl_loop_main_get(EFL_LOOP_CLASS);
}

EOLIAN static void
_efl_loop_iterate(Eo *obj, Efl_Loop_Data *pd)
{
   _ecore_main_loop_iterate(obj, pd);
}

EOLIAN static int
_efl_loop_iterate_may_block(Eo *obj, Efl_Loop_Data *pd, int may_block)
{
   return _ecore_main_loop_iterate_may_block(obj, pd, may_block);
}

// FIXME: This should return an Eina_Value, but that doesn't work at the moment
EOLIAN static Eina_Value *
_efl_loop_begin(Eo *obj, Efl_Loop_Data *pd)
{
   _ecore_main_loop_begin(obj, pd);
   return &(pd->exit_code);
}

EOLIAN static void
_efl_loop_quit(Eo *obj, Efl_Loop_Data *pd, Eina_Value exit_code)
{
   _ecore_main_loop_quit(obj, pd);
   pd->exit_code = exit_code;
}

EOLIAN static void
_efl_loop_time_set(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd, double t)
{
   pd->loop_time = t;
}

EOLIAN static double
_efl_loop_time_get(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd)
{
   return pd->loop_time;
}

EAPI void
efl_exit(int exit_code)
{
   Eina_Value v = EINA_VALUE_EMPTY;

   eina_value_setup(&v, EINA_VALUE_TYPE_INT);
   eina_value_set(&v, &exit_code);
   efl_loop_quit(ecore_main_loop_get(), v);
}

EOLIAN static Efl_Object *
_efl_loop_efl_object_provider_find(Eo *obj, Efl_Loop_Data *pd, const Efl_Object *klass)
{
   Efl_Object *r;

   if (klass == EFL_LOOP_CLASS) return obj;

   r = eina_hash_find(pd->providers, &klass);
   if (r) return r;

   return efl_provider_find(efl_super(obj, EFL_LOOP_CLASS), klass);
}

EAPI int
efl_loop_exit_code_process(Eina_Value *value)
{
   const Eina_Value_Type *t = eina_value_type_get(value);
   int r = 0;

   if (t == EINA_VALUE_TYPE_UCHAR ||
       t == EINA_VALUE_TYPE_USHORT ||
       t == EINA_VALUE_TYPE_UINT ||
       t == EINA_VALUE_TYPE_ULONG ||
       t == EINA_VALUE_TYPE_UINT64 ||
       t == EINA_VALUE_TYPE_CHAR ||
       t == EINA_VALUE_TYPE_SHORT ||
       t == EINA_VALUE_TYPE_INT ||
       t == EINA_VALUE_TYPE_LONG ||
       t == EINA_VALUE_TYPE_INT64 ||
       t == EINA_VALUE_TYPE_FLOAT ||
       t == EINA_VALUE_TYPE_DOUBLE)
     {
        Eina_Value v = EINA_VALUE_EMPTY;

        eina_value_setup(&v, EINA_VALUE_TYPE_INT);
        if (!eina_value_convert(&v, value)) r = -1;
        else eina_value_get(&v, &v);
     }
   else
     {
        FILE *out = stdout;
        char *msg;

        msg = eina_value_to_string(value);

        if (t == EINA_VALUE_TYPE_ERROR)
          {
             r = -1;
             out = stderr;
          }
        fprintf(out, "%s\n", msg);
     }
   return r;
}

static void
_poll_trigger(void *data, const Efl_Event *event)
{
   Eo *parent = efl_parent_get(event->object);

   efl_event_callback_call(parent, data, NULL);
}

static void
_check_event_catcher_add(void *data, const Efl_Event *event)
{
   const Efl_Callback_Array_Item *array = event->info;
   Efl_Loop_Data *pd = data;
   int i;

   for (i = 0; array[i].desc != NULL; i++)
     {
        if (array[i].desc == EFL_LOOP_EVENT_IDLE)
          {
             ++pd->idlers;
          }
        // XXX: all the below are kind of bad. ecore_pollers were special.
        // they all woke up at the SAME time based on interval, (all pollers
        // of interval 1 woke up together, those with 2 woke up when 1 and
        // 2 woke up, 4 woke up together along with 1 and 2 etc.
        // the below means they will just go off whenever but at a pre
        // defined interval - 1/60th, 6 and 66 seconds. not really great
        // pollers probably should be less frequent that 1/60th even on poll
        // high, medium probably down to 1-2 sec and low - yes maybe 30 or 60
        // sec... still - not timed to wake up together. :(
        else if (array[i].desc == EFL_LOOP_EVENT_POLL_HIGH)
          {
             if (!pd->poll_high)
               {
                  // Would be better to have it in sync with normal wake up
                  // of the main loop for better energy efficiency, I guess.
                  pd->poll_high = efl_add
                    (EFL_LOOP_TIMER_CLASS, event->object,
                     efl_event_callback_add(efl_added,
                                            EFL_LOOP_TIMER_EVENT_TICK,
                                            _poll_trigger,
                                            EFL_LOOP_EVENT_POLL_HIGH),
                     efl_loop_timer_interval_set(efl_added, 1.0 / 60.0));
               }
             ++pd->pollers.high;
          }
        else if (array[i].desc == EFL_LOOP_EVENT_POLL_MEDIUM)
          {
             if (!pd->poll_medium)
               {
                  pd->poll_medium = efl_add
                    (EFL_LOOP_TIMER_CLASS, event->object,
                     efl_event_callback_add(efl_added,
                                            EFL_LOOP_TIMER_EVENT_TICK,
                                            _poll_trigger,
                                            EFL_LOOP_EVENT_POLL_MEDIUM),
                     efl_loop_timer_interval_set(efl_added, 6));
               }
             ++pd->pollers.medium;
          }
        else if (array[i].desc == EFL_LOOP_EVENT_POLL_LOW)
          {
             if (!pd->poll_low)
               {
                  pd->poll_low = efl_add
                    (EFL_LOOP_TIMER_CLASS, event->object,
                     efl_event_callback_add(efl_added,
                                            EFL_LOOP_TIMER_EVENT_TICK,
                                            _poll_trigger,
                                            EFL_LOOP_EVENT_POLL_LOW),
                     efl_loop_timer_interval_set(efl_added, 66));
               }
             ++pd->pollers.low;
          }
     }
}

static void
_check_event_catcher_del(void *data, const Efl_Event *event)
{
   const Efl_Callback_Array_Item *array = event->info;
   Efl_Loop_Data *pd = data;
   int i;

   for (i = 0; array[i].desc != NULL; i++)
     {
        if (array[i].desc == EFL_LOOP_EVENT_IDLE)
          {
             --pd->idlers;
          }
        else if (array[i].desc == EFL_LOOP_EVENT_POLL_HIGH)
          {
             --pd->pollers.high;
             if (!pd->pollers.high)
               {
                  efl_del(pd->poll_high);
                  pd->poll_high = NULL;
               }
          }
        else if (array[i].desc == EFL_LOOP_EVENT_POLL_MEDIUM)
          {
             --pd->pollers.medium;
             if (!pd->pollers.medium)
               {
                  efl_del(pd->poll_medium);
                  pd->poll_medium = NULL;
               }
          }
        else if (array[i].desc == EFL_LOOP_EVENT_POLL_LOW)
          {
             --pd->pollers.low;
             if (!pd->pollers.low)
               {
                  efl_del(pd->poll_low);
                  pd->poll_low = NULL;
               }
          }
     }
}

EFL_CALLBACKS_ARRAY_DEFINE(event_catcher_watch,
                          { EFL_EVENT_CALLBACK_ADD, _check_event_catcher_add },
                          { EFL_EVENT_CALLBACK_DEL, _check_event_catcher_del });

EOLIAN static Efl_Object *
_efl_loop_efl_object_constructor(Eo *obj, Efl_Loop_Data *pd)
{
   obj = efl_constructor(efl_super(obj, EFL_LOOP_CLASS));
   if (!obj) return NULL;

   efl_event_callback_array_add(obj, event_catcher_watch(), pd);

   pd->loop_time = ecore_time_get();
   pd->providers = eina_hash_pointer_new(EINA_FREE_CB(efl_unref));
   pd->message_handlers = eina_inarray_new(sizeof(Message_Handler), 32);
   pd->epoll_fd = -1;
   pd->timer_fd = -1;
   return obj;
}

EOLIAN static void
_efl_loop_efl_object_destructor(Eo *obj, Efl_Loop_Data *pd)
{
   _ecore_main_content_clear(pd);

   eina_hash_free(pd->providers);
   pd->providers = NULL;

   efl_del(pd->poll_low);
   pd->poll_low = NULL;
   efl_del(pd->poll_medium);
   pd->poll_medium = NULL;
   efl_del(pd->poll_high);
   pd->poll_high = NULL;

   eina_inarray_free(pd->message_handlers);
   pd->message_handlers = NULL;

   efl_destructor(efl_super(obj, EFL_LOOP_CLASS));

   if (obj == _mainloop_singleton)
     {
        _mainloop_singleton = NULL;
        _mainloop_singleton_data = NULL;
     }
}

static void
_efl_loop_arguments_cleanup(Eina_Array *arga)
{
   Eina_Stringshare *s;

   while ((s = eina_array_pop(arga))) eina_stringshare_del(s);
   eina_array_free(arga);
}

static Eina_Value
_efl_loop_arguments_send(void *data, const Eina_Value v,
                         const Eina_Future *dead EINA_UNUSED)

{
   static Eina_Bool initialization = EINA_TRUE;
   Efl_Loop_Arguments arge;
   Eina_Array *arga = data;

   if (v.type == EINA_VALUE_TYPE_ERROR) goto on_error;

   arge.argv = arga;
   arge.initialization = initialization;
   initialization = EINA_FALSE;

   efl_event_callback_call(ecore_main_loop_get(),
                           EFL_LOOP_EVENT_ARGUMENTS, &arge);
on_error:
   _efl_loop_arguments_cleanup(arga);
   return v;
}

// It doesn't make sense to send those argument to any other mainloop
// As it also doesn't make sense to allow anyone to override this, so
// should be internal for sure, not even protected.
EAPI void
ecore_loop_arguments_send(int argc, const char **argv)
{
   Eina_Future *job;
   Eina_Array *arga;
   int i = 0;

   arga = eina_array_new(argc);
   for (i = 0; i < argc; i++)
     eina_array_push(arga, eina_stringshare_add(argv[i]));

   job = eina_future_then(efl_loop_job(ecore_main_loop_get()),
                          _efl_loop_arguments_send, arga);
   efl_future_Eina_FutureXXX_then(ecore_main_loop_get(), job);
}

// Only one main loop handle for now
void
ecore_loop_future_register(Efl_Loop *l, Efl_Future *f)
{
   Efl_Loop_Data *pd;

   if (l == ML_OBJ) pd = ML_DAT;
   else pd = efl_data_scope_get(l, EFL_LOOP_CLASS);
   if (!pd) return;

   pd->pending_futures = eina_list_append(pd->pending_futures, f);
}

void
ecore_loop_future_unregister(Efl_Loop *l, Efl_Future *f)
{
   Efl_Loop_Data *pd;

   if (l == ML_OBJ) pd = ML_DAT;
   else pd = efl_data_scope_get(l, EFL_LOOP_CLASS);
   if (!pd) return;

   pd->pending_futures = eina_list_remove(pd->pending_futures, f);
}

void
ecore_loop_promise_register(Efl_Loop *l, Efl_Promise *p)
{
   Efl_Loop_Data *pd;

   if (l == ML_OBJ) pd = ML_DAT;
   else pd = efl_data_scope_get(l, EFL_LOOP_CLASS);
   if (!pd) return;

   pd->pending_promises = eina_list_append(pd->pending_promises, p);
}

void
ecore_loop_promise_unregister(Efl_Loop *l, Efl_Promise *p)
{
   Efl_Loop_Data *pd;

   if (l == ML_OBJ) pd = ML_DAT;
   else pd = efl_data_scope_get(l, EFL_LOOP_CLASS);
   if (!pd) return;

   pd->pending_promises = eina_list_remove(pd->pending_promises, p);
}

static Eina_Future *
_efl_loop_job(Eo *obj, Efl_Loop_Data *pd EINA_UNUSED)
{
   // NOTE: Eolian should do efl_future_then() to bind future to object.
   return efl_future_Eina_FutureXXX_then
     (obj, eina_future_resolved(efl_loop_future_scheduler_get(obj),
                                EINA_VALUE_EMPTY));
}

EOLIAN static void
_efl_loop_throttle_set(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd, double amount)
{
   pd->throttle = ((double)amount) * 1000000.0;
}

EOLIAN static double
_efl_loop_throttle_get(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd)
{
   return (double)(pd->throttle) / 1000000.0;
}

static void
_efl_loop_idle_cancel(void *data, const Eina_Promise *dead_ptr EINA_UNUSED)
{
   Efl_Loop_Promise_Simple_Data *d = data;
   ecore_idler_del(d->idler);
   efl_loop_promise_simple_data_mp_free(d);
}

static Eina_Bool
_efl_loop_idle_done(void *data)
{
   Efl_Loop_Promise_Simple_Data *d = data;
   eina_promise_resolve(d->promise, EINA_VALUE_EMPTY);
   efl_loop_promise_simple_data_mp_free(d);
   return EINA_FALSE;
}

static Eina_Future *
_efl_loop_idle(Eo *obj, Efl_Loop_Data *pd EINA_UNUSED)
{
   Efl_Loop_Promise_Simple_Data *d;
   Eina_Promise *p;

   d = efl_loop_promise_simple_data_calloc(1);
   EINA_SAFETY_ON_NULL_RETURN_VAL(d, NULL);

   d->idler = ecore_idler_add(_efl_loop_idle_done, d);
   EINA_SAFETY_ON_NULL_GOTO(d->idler, idler_error);

   p = eina_promise_new(efl_loop_future_scheduler_get(obj),
                        _efl_loop_idle_cancel, d);
   // d is dead if p is NULL
   EINA_SAFETY_ON_NULL_RETURN_VAL(p, NULL);
   d->promise = p;

   // NOTE: Eolian should do efl_future_then() to bind future to object.
   return efl_future_Eina_FutureXXX_then(obj, eina_future_new(p));

idler_error:
   efl_loop_promise_simple_data_mp_free(d);
   return NULL;
}

static void
_efl_loop_timeout_cancel(void *data, const Eina_Promise *dead_ptr EINA_UNUSED)
{
   Efl_Loop_Promise_Simple_Data *d = data;
   efl_del(d->timer);
   d->timer = NULL;
   efl_loop_promise_simple_data_mp_free(d);
}

static void
_efl_loop_timeout_done(void *data, const Efl_Event *event)
{
   Efl_Loop_Promise_Simple_Data *d = data;

   eina_promise_resolve(d->promise, EINA_VALUE_EMPTY);
   d->timer = NULL;
   efl_loop_promise_simple_data_mp_free(d);
   efl_del(event->object);
}

static Eina_Future *
_efl_loop_timeout(Eo *obj, Efl_Loop_Data *pd EINA_UNUSED, double tim)
{
   Efl_Loop_Promise_Simple_Data *d;
   Eina_Promise *p;

   d = efl_loop_promise_simple_data_calloc(1);
   EINA_SAFETY_ON_NULL_RETURN_VAL(d, NULL);

   d->timer = efl_add(EFL_LOOP_TIMER_CLASS, obj,
                      efl_loop_timer_interval_set(efl_added, tim),
                      efl_event_callback_add(efl_added,
                                             EFL_LOOP_TIMER_EVENT_TICK,
                                             _efl_loop_timeout_done, d));
   EINA_SAFETY_ON_NULL_GOTO(d->timer, timer_error);

   p = eina_promise_new(efl_loop_future_scheduler_get(obj),
                        _efl_loop_timeout_cancel, d);
   // d is dead if p is NULL
   EINA_SAFETY_ON_NULL_RETURN_VAL(p, NULL);
   d->promise = p;

   // NOTE: Eolian should do efl_future_then() to bind future to object.
   return efl_future_Eina_FutureXXX_then(obj, eina_future_new(p));

timer_error:
   efl_loop_promise_simple_data_mp_free(d);
   return NULL;
}

static Eina_Bool
_efl_loop_register(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd, const Efl_Class *klass, const Efl_Object *provider)
{
   // The passed object does not provide that said class.
   if (!efl_isa(provider, klass)) return EINA_FALSE;

   // Note: I would prefer to use efl_xref here, but I can't figure a nice way to
   // call efl_xunref on hash destruction.
   return eina_hash_add(pd->providers, &klass, efl_ref(provider));
}

static Eina_Bool
_efl_loop_unregister(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd, const Efl_Class *klass, const Efl_Object *provider)
{
   return eina_hash_del(pd->providers, &klass, provider);
}

void
_efl_loop_messages_filter(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd, void *handler_pd)
{
   Message *msg;

   pd->message_walking++;
   EINA_INLIST_FOREACH(pd->message_queue, msg)
     {
        if ((msg->handler) && (msg->message) && (!msg->delete_me))
          {
             if (!_ecore_event_do_filter(handler_pd,
                                         msg->handler, msg->message))
               {
                  efl_del(msg->message);
                  msg->handler = NULL;
                  msg->message = NULL;
                  msg->delete_me = EINA_TRUE;
               }
          }
     }
   pd->message_walking--;
}

void
_efl_loop_messages_call(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd, void *func, void *data)
{
   Message *msg;

   pd->message_walking++;
   EINA_INLIST_FOREACH(pd->message_queue, msg)
     {
        if ((msg->handler) && (msg->message) && (!msg->delete_me))
          {
             Eina_Bool (*fn) (void *data, void *handler, void *msg);

             fn = func;
             if (!fn(data, msg->handler, msg->message))
               {
                  efl_del(msg->message);
                  msg->handler = NULL;
                  msg->message = NULL;
                  msg->delete_me = EINA_TRUE;
               }
          }
     }
   pd->message_walking--;
}

EOLIAN static Eina_Bool
_efl_loop_message_process(Eo *obj, Efl_Loop_Data *pd)
{
   if (!pd->message_queue) return EINA_FALSE;
   pd->message_walking++;
   _ecore_event_filters_call(obj, pd);
   while (pd->message_queue)
     {
        Message *msg = (Message *)pd->message_queue;
        if (!msg->delete_me)
          efl_loop_message_handler_message_call(msg->handler, msg->message);
        else
          {
             if (msg->message) efl_del(msg->message);
             pd->message_queue =
               eina_inlist_remove(pd->message_queue,
                                  pd->message_queue);
             free(msg);
          }
     }
   pd->message_walking--;
   if (pd->message_walking == 0)
     {
        Message *msg;

        EINA_INLIST_FREE(pd->message_queue, msg)
          {
             if (msg->message)
               {
                  if (!msg->delete_me)
                    ERR("Found unprocessed event msg=%p handler=%p on queue",
                        msg->message, msg->handler);
                  efl_del(msg->message);
               }
             else free(msg);
          }
     }
   return EINA_TRUE;
}

EOAPI EFL_FUNC_BODY(efl_loop_message_process, Eina_Bool, 0);

EWAPI void
efl_build_version_set(int vmaj, int vmin, int vmic, int revision,
                      const char *flavor, const char *build_id)
{
   // note: EFL has not been initialized yet at this point (ie. no eina call)
   _app_efl_version.major = vmaj;
   _app_efl_version.minor = vmin;
   _app_efl_version.micro = vmic;
   _app_efl_version.revision = revision;
   free((char *)_app_efl_version.flavor);
   free((char *)_app_efl_version.build_id);
   _app_efl_version.flavor = flavor ? strdup(flavor) : NULL;
   _app_efl_version.build_id = build_id ? strdup(build_id) : NULL;
}

EOLIAN static const Efl_Version *
_efl_loop_app_efl_version_get(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd EINA_UNUSED)
{
   return &_app_efl_version;
}

EOLIAN static const Efl_Version *
_efl_loop_efl_version_get(Eo *obj EINA_UNUSED, Efl_Loop_Data *pd EINA_UNUSED)
{
   /* vanilla EFL: flavor = NULL */
   static const Efl_Version version = {
      .major = VMAJ,
      .minor = VMIN,
      .micro = VMIC,
      .revision = VREV,
      .build_id = EFL_BUILD_ID,
      .flavor = NULL
   };
   return &version;
}

EAPI Eina_Future_Scheduler *
efl_loop_future_scheduler_get(Eo *obj)
{
   if (!obj) return NULL;

   if (efl_isa(obj, EFL_LOOP_CLASS))
     return _ecore_event_future_scheduler_get();

   return efl_loop_future_scheduler_get(efl_loop_get(obj));
}

#define EFL_LOOP_EXTRA_OPS \
  EFL_OBJECT_OP_FUNC(efl_loop_message_process, _efl_loop_message_process)

#include "efl_loop.eo.c"
