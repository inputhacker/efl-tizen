#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <string.h>

#include "Ecore_Input.h"

#include "ecore_private.h"
#include "ecore_input_private.h"


int _ecore_input_log_dom = -1;

EAPI int ECORE_EVENT_KEY_DOWN = 0;
EAPI int ECORE_EVENT_KEY_UP = 0;
EAPI int ECORE_EVENT_MOUSE_BUTTON_DOWN = 0;
EAPI int ECORE_EVENT_MOUSE_BUTTON_UP = 0;
EAPI int ECORE_EVENT_MOUSE_MOVE = 0;
EAPI int ECORE_EVENT_MOUSE_WHEEL = 0;
EAPI int ECORE_EVENT_MOUSE_IN = 0;
EAPI int ECORE_EVENT_MOUSE_OUT = 0;
EAPI int ECORE_EVENT_AXIS_UPDATE = 0;
EAPI int ECORE_EVENT_MOUSE_BUTTON_CANCEL = 0;
// TIZEN_ONLY(20171206): add dummy functions for ecore_device API
EAPI int ECORE_EVENT_DEVICE_ADD = 0;
EAPI int ECORE_EVENT_DEVICE_DEL = 0;
//
EAPI int ECORE_EVENT_JOYSTICK = 0;
// TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
EAPI int ECORE_EVENT_DETENT_ROTATE = 0;
//
// TIZEN_ONLY(20180917): ecore/evas_device: update device info if subclas is changed
EAPI int ECORE_EVENT_DEVICE_SUBCLASS_UPDATE = 0;
//


static int _ecore_event_init_count = 0;

EAPI int
ecore_event_init(void)
{
   if (++_ecore_event_init_count != 1)
     return _ecore_event_init_count;
   if (!ecore_init())
     {
        _ecore_event_init_count--;
        return 0;
     }

   _ecore_input_log_dom = eina_log_domain_register
     ("ecore_input", ECORE_INPUT_DEFAULT_LOG_COLOR);
   if(_ecore_input_log_dom < 0)
     {
       EINA_LOG_ERR("Impossible to create a log domain for the ecore input module.");
       return --_ecore_event_init_count;
     }

   ECORE_EVENT_KEY_DOWN = ecore_event_type_new();
   ECORE_EVENT_KEY_UP = ecore_event_type_new();
   ECORE_EVENT_MOUSE_BUTTON_DOWN = ecore_event_type_new();
   ECORE_EVENT_MOUSE_BUTTON_UP = ecore_event_type_new();
   ECORE_EVENT_MOUSE_MOVE = ecore_event_type_new();
   ECORE_EVENT_MOUSE_WHEEL = ecore_event_type_new();
   ECORE_EVENT_MOUSE_IN = ecore_event_type_new();
   ECORE_EVENT_MOUSE_OUT = ecore_event_type_new();
   ECORE_EVENT_AXIS_UPDATE = ecore_event_type_new();
   ECORE_EVENT_MOUSE_BUTTON_CANCEL = ecore_event_type_new();
   ECORE_EVENT_JOYSTICK = ecore_event_type_new();
   // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
   ECORE_EVENT_DETENT_ROTATE = ecore_event_type_new();
   //
   // TIZEN_ONLY(20180118): support a Ecore_Device
   ECORE_EVENT_DEVICE_ADD = ecore_event_type_new();
   ECORE_EVENT_DEVICE_DEL = ecore_event_type_new();
   //
   // TIZEN_ONLY(20180917): ecore/evas_device: update device info if subclas is changed
   ECORE_EVENT_DEVICE_SUBCLASS_UPDATE = ecore_event_type_new();
   //

   //TIZEN_ONLY(20170307) Remove warning message
   #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
   if (getenv("ECORE_INPUT_ENABLE_JOYSTICK_INIT"))
     ecore_input_joystick_init();
   #pragma GCC diagnostic pop
   //

   return _ecore_event_init_count;
}

EAPI int
ecore_event_shutdown(void)
{
   if (--_ecore_event_init_count != 0)
     return _ecore_event_init_count;

   ecore_event_type_flush(ECORE_EVENT_KEY_DOWN,
                          ECORE_EVENT_KEY_UP,
                          ECORE_EVENT_MOUSE_BUTTON_DOWN,
                          ECORE_EVENT_MOUSE_BUTTON_UP,
                          ECORE_EVENT_MOUSE_MOVE,
                          ECORE_EVENT_MOUSE_WHEEL,
                          ECORE_EVENT_MOUSE_IN,
                          ECORE_EVENT_MOUSE_OUT,
                          ECORE_EVENT_AXIS_UPDATE,
                          ECORE_EVENT_MOUSE_BUTTON_CANCEL,
                          ECORE_EVENT_JOYSTICK,
                          // TIZEN_ONLY(20171109): support a tizen_input_device_manager interface
                          ECORE_EVENT_DETENT_ROTATE,
                          //
                          // TIZEN_ONLY(20180118): support a Ecore_Device
                          ECORE_EVENT_DEVICE_ADD,
                          ECORE_EVENT_DEVICE_DEL,
                          //
                          // TIZEN_ONLY(20180917): ecore/evas_device: update device info if subclas is changed
                          ECORE_EVENT_DEVICE_SUBCLASS_UPDATE
                          //
                          );
   //TIZEN_ONLY(20170307) Remove warning message
   #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
   ecore_input_joystick_shutdown();
   #pragma GCC diagnostic pop
   //
   eina_log_domain_unregister(_ecore_input_log_dom);
   _ecore_input_log_dom = -1;
   ecore_shutdown();
   return _ecore_event_init_count;
}

typedef struct _Ecore_Event_Modifier_Match Ecore_Event_Modifier_Match;
struct _Ecore_Event_Modifier_Match
{
   const char *key;
   Ecore_Event_Modifier modifier;
   unsigned int event_modifier;
};

static const Ecore_Event_Modifier_Match matchs[] = {
  { "Shift_L", ECORE_SHIFT, ECORE_EVENT_MODIFIER_SHIFT },
  { "Shift_R", ECORE_SHIFT, ECORE_EVENT_MODIFIER_SHIFT },
  { "Alt_L", ECORE_ALT, ECORE_EVENT_MODIFIER_ALT },
  { "Alt_R", ECORE_ALT, ECORE_EVENT_MODIFIER_ALT },
  { "Control_L", ECORE_CTRL, ECORE_EVENT_MODIFIER_CTRL },
  { "Control_R", ECORE_CTRL, ECORE_EVENT_MODIFIER_CTRL },
  { "Caps_Lock", ECORE_CAPS, ECORE_EVENT_MODIFIER_CAPS },
  { "Super_L", ECORE_WIN, ECORE_EVENT_MODIFIER_WIN },
  { "Super_R", ECORE_WIN, ECORE_EVENT_MODIFIER_WIN },
  { "ISO_Level3_Shift", ECORE_MODE, ECORE_EVENT_MODIFIER_ALTGR },
  { "Scroll_Lock", ECORE_SCROLL, ECORE_EVENT_MODIFIER_SCROLL }
};

EAPI unsigned int
ecore_event_modifier_mask(Ecore_Event_Modifier modifier)
{
   size_t i;

   for (i = 0; i < sizeof (matchs) / sizeof (Ecore_Event_Modifier_Match); i++)
     if (matchs[i].modifier == modifier)
       return matchs[i].event_modifier;

   return 0;
}

EAPI Ecore_Event_Modifier
ecore_event_update_modifier(const char *key, Ecore_Event_Modifiers *modifiers, int inc)
{
   size_t i;

   for (i = 0; i < sizeof (matchs) / sizeof (Ecore_Event_Modifier_Match); i++)
     if (strcmp(matchs[i].key, key) == 0)
       {
          if (modifiers && matchs[i].modifier < modifiers->size)
            modifiers->array[matchs[i].modifier] += inc;
          return matchs[i].modifier;
       }

   return ECORE_NONE;
}
