#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#include "Ecore.h"
#include "ecore_private.h"

typedef struct _Ecore_Future_Schedule_Entry
{
   Eina_Future_Schedule_Entry base;
   Eina_Future_Scheduler_Cb cb;
   Eina_Future *future;
   Eina_Value value;
} Ecore_Future_Schedule_Entry;

//////
// XXX: still using legacy ecore events
static Eina_Bool            shutting_down              = EINA_FALSE;
static Eina_Mempool        *mp_future_schedule_entry   = NULL;
//
//////

static Ecore_Event_Message_Handler *_event_msg_handler = NULL;

EAPI Ecore_Event_Handler *
ecore_event_handler_add(int                    type,
                        Ecore_Event_Handler_Cb func,
                        const void            *data)
{
   return ecore_event_message_handler_add(_event_msg_handler,
                                          type, func, (void *)data);
}

EAPI Ecore_Event_Handler *
ecore_event_handler_prepend(int                    type,
                        Ecore_Event_Handler_Cb func,
                        const void            *data)
{
   return ecore_event_message_handler_prepend(_event_msg_handler,
                                          type, func, (void *)data);
}

EAPI void *
ecore_event_handler_del(Ecore_Event_Handler *event_handler)
{
   return ecore_event_message_handler_del(_event_msg_handler,
                                          event_handler);
}

EAPI void *
ecore_event_handler_data_get(Ecore_Event_Handler *eh)
{
   return ecore_event_message_handler_data_get(_event_msg_handler, eh);
}

EAPI void *
ecore_event_handler_data_set(Ecore_Event_Handler *eh,
                             const void          *data)
{
   return ecore_event_message_handler_data_set(_event_msg_handler, eh,
                                               (void *)data);
}

EAPI Ecore_Event *
ecore_event_add(int          type,
                void        *ev,
                Ecore_End_Cb func_free,
                void        *data)
{
   Ecore_Event_Message *msg;
   if (type <= ECORE_EVENT_NONE) return NULL;

   //TIZEN_ONLY(20180510): fix backword compatibility bug for ecore event
   msg = ecore_event_message_handler_message_type_add(_event_msg_handler, type);
   //
   if (msg)
     {
        ecore_event_message_data_set(msg, type, ev, func_free, data);
        efl_loop_message_handler_message_send(_event_msg_handler, msg);
     }
   return (Ecore_Event *)msg;
}

EAPI void *
ecore_event_del(Ecore_Event *event)
{
   void *data = NULL;
   if (!event) return data;
   ecore_event_message_data_get((Eo *)event, NULL, NULL, NULL, &data);
   _efl_loop_message_unsend((Eo *)event);
   return data;
}

EAPI int
ecore_event_type_new(void)
{
   return ecore_event_message_handler_type_new(_event_msg_handler);
}

EAPI Ecore_Event_Filter *
ecore_event_filter_add(Ecore_Data_Cb   func_start,
                       Ecore_Filter_Cb func_filter,
                       Ecore_End_Cb    func_end,
                       const void     *data)
{
   return ecore_event_message_handler_filter_add(_event_msg_handler,
                                                 func_start, func_filter,
                                                 func_end, (void *)data);
}

EAPI void *
ecore_event_filter_del(Ecore_Event_Filter *ef)
{
   return ecore_event_message_handler_filter_del(_event_msg_handler, ef);
}

EAPI int
ecore_event_current_type_get(void)
{
   return ecore_event_message_handler_current_type_get(_event_msg_handler);
}

EAPI void *
ecore_event_current_event_get(void)
{
   return ecore_event_message_handler_current_event_get(_event_msg_handler);
}

static void _future_dispatch_cb(void *data, const Efl_Event *ev EINA_UNUSED);
static void _event_del_cb(void *data, const Efl_Event *ev);

EFL_CALLBACKS_ARRAY_DEFINE(ecore_future_callbacks,
                           { EFL_LOOP_EVENT_IDLE_ENTER, _future_dispatch_cb },
                           { EFL_LOOP_EVENT_IDLE, _future_dispatch_cb },
                           { EFL_EVENT_DEL, _event_del_cb });

static void
_future_dispatch_cb(void *data, const Efl_Event *ev EINA_UNUSED)
{
   Efl_Loop_Future_Scheduler *loopsched = data;
   Eina_List *entries = loopsched->future_entries;
   Ecore_Future_Schedule_Entry *entry;

   loopsched->future_entries = NULL;
   efl_event_callback_array_del((Eo *) loopsched->loop, ecore_future_callbacks(), loopsched);

   EINA_LIST_FREE(entries, entry)
     {
        entry->cb(entry->future, entry->value);
        eina_mempool_free(mp_future_schedule_entry, entry);
     }
}

static void
_event_del_cb(void *data, const Efl_Event *ev EINA_UNUSED)
{
   Efl_Loop_Future_Scheduler *loopsched = data;
   Eina_List *entries = loopsched->future_entries;
   Ecore_Future_Schedule_Entry *entry;

   loopsched->future_entries = NULL;
   efl_event_callback_array_del((Eo *) loopsched->loop, ecore_future_callbacks(), loopsched);

   EINA_LIST_FREE(entries, entry)
     {
        eina_future_cancel(entry->future);
        eina_value_flush(&entry->value);
        eina_mempool_free(mp_future_schedule_entry, entry);
     }
}

static Eina_Future_Schedule_Entry *
ecore_future_schedule(Eina_Future_Scheduler *sched,
                      Eina_Future_Scheduler_Cb cb,
                      Eina_Future *future,
                      Eina_Value value)
{
   Efl_Loop_Future_Scheduler *loopsched = (Efl_Loop_Future_Scheduler *)sched;
   Ecore_Future_Schedule_Entry *entry;

   entry = eina_mempool_malloc(mp_future_schedule_entry, sizeof(*entry));
   EINA_SAFETY_ON_NULL_RETURN_VAL(entry, NULL);
   entry->base.scheduler = sched;
   entry->cb = cb;
   entry->future = future;
   entry->value = value;

   if (!loopsched->future_entries)
     efl_event_callback_array_add((Eo *) loopsched->loop, ecore_future_callbacks(), loopsched);

   loopsched->future_entries = eina_list_append(loopsched->future_entries, entry);
   return &entry->base;
}

static void
ecore_future_recall(Eina_Future_Schedule_Entry *s_entry)
{
   Ecore_Future_Schedule_Entry *entry = (Ecore_Future_Schedule_Entry *)s_entry;
   Efl_Loop_Future_Scheduler *loopsched;
   Eina_List *lookup;

   if (shutting_down) return;

   loopsched = (Efl_Loop_Future_Scheduler *) entry->base.scheduler;

   lookup = eina_list_data_find_list(loopsched->future_entries, entry);
   if (!lookup) return;

   loopsched->future_entries = eina_list_remove_list(loopsched->future_entries, lookup);
   if (!loopsched->future_entries)
     efl_event_callback_array_del((Eo *) loopsched->loop, ecore_future_callbacks(), loopsched);

   eina_value_flush(&entry->value);
   eina_mempool_free(mp_future_schedule_entry, entry);

}

static Eina_Future_Scheduler ecore_future_scheduler = {
   .schedule = ecore_future_schedule,
   .recall = ecore_future_recall,
};

Eina_Future_Scheduler *
_ecore_event_future_scheduler_get(void)
{
   return &ecore_future_scheduler;
}

Eina_Bool
_ecore_event_init(void)
{
   const char *choice = getenv("EINA_MEMPOOL");
   if ((!choice) || (!choice[0])) choice = "chained_mempool";

   _event_msg_handler = efl_add(ECORE_EVENT_MESSAGE_HANDLER_CLASS, _mainloop_singleton);
   efl_loop_register(_mainloop_singleton, ECORE_EVENT_MESSAGE_HANDLER_CLASS, _event_msg_handler);

   if (!_event_msg_handler)
     {
        ERR("Cannot create legacy ecore event message handler");
        return EINA_FALSE;
     }
   // init some core legacy event types in t he same order and numbering as before
   // ECORE_EVENT_NONE                     0
   // no need to do as ev types start at 1

   // ECORE_EVENT_SIGNAL_USER              1
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_SIGNAL_HUP               2
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_SIGNAL_EXIT              3
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_SIGNAL_POWER             4
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_SIGNAL_REALTIME          5
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_MEMORY_STATE             6
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_POWER_STATE              7
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_LOCALE_CHANGED           8
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_HOSTNAME_CHANGED         9
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_SYSTEM_TIMEDATE_CHANGED  10
   ecore_event_message_handler_type_new(_event_msg_handler);
   // ECORE_EVENT_COUNT                    11
   // no need to do as it was a count, nto an event

   //////
   // XXX: ecore future still using legacy...
   shutting_down = EINA_FALSE;
//   ECORE_EV_FUTURE_ID = ecore_event_type_new();
//   future_handler = ecore_event_handler_add(ECORE_EV_FUTURE_ID, ecore_future_dispatched, NULL);
//   EINA_SAFETY_ON_NULL_GOTO(future_handler, err_handler);
   //FIXME: Is 512 too high?
   if (!mp_future_schedule_entry)
     {
        mp_future_schedule_entry = eina_mempool_add
          (choice, "Ecore_Future_Event", NULL,
           sizeof(Ecore_Future_Schedule_Entry), 512);
        EINA_SAFETY_ON_NULL_GOTO(mp_future_schedule_entry, err_pool);
     }
   //
   //////

   return EINA_TRUE;

 err_pool:
// XXX:
//   ecore_event_handler_del(future_handler);
//   future_handler = NULL;
// err_handler:
//   ECORE_EV_FUTURE_ID = -1;
   return EINA_FALSE;
}

void
_ecore_event_shutdown(void)
{
   shutting_down = EINA_TRUE;

   //////
   // XXX: ecore future still using legacy...
//   ecore_event_handler_del(future_handler);
//   future_handler = NULL;
//   ECORE_EV_FUTURE_ID = -1;
   //
   //////

   efl_loop_message_handler_message_clear(_event_msg_handler);
   _event_msg_handler = NULL;
}

void *
_ecore_event_signal_user_new(void)
{
   return calloc(1, sizeof(Ecore_Event_Signal_User));
}

void *
_ecore_event_signal_hup_new(void)
{
   return calloc(1, sizeof(Ecore_Event_Signal_Hup));
}

void *
_ecore_event_signal_exit_new(void)
{
   return calloc(1, sizeof(Ecore_Event_Signal_Exit));
}

void *
_ecore_event_signal_power_new(void)
{
   return calloc(1, sizeof(Ecore_Event_Signal_Power));
}

void *
_ecore_event_signal_realtime_new(void)
{
   return calloc(1, sizeof(Ecore_Event_Signal_Realtime));
}

EAPI void
ecore_event_type_flush_internal(int type, ...)
{
   va_list args;

   if (type == ECORE_EVENT_NONE) return;
   ecore_event_message_handler_type_flush(_event_msg_handler, type);

   va_start(args, type);
   for (;;)
     {
        type = va_arg(args, int);
        if (type == ECORE_EVENT_NONE) break;
        ecore_event_message_handler_type_flush(_event_msg_handler, type);
     }
   va_end(args);
}
