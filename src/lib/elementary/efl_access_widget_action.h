#ifndef ELM_INTERFACE_ATSPI_WIDGET_ACTION_H
#define ELM_INTERFACE_ATSPI_WIDGET_ACTION_H

// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#ifdef EFL_BETA_API_SUPPORT
//

struct _Efl_Access_Action_Data
{
   const char *name;
   const char *action;
   const char *param;
   Eina_Bool (*func)(Evas_Object *obj, const char *params);
};

typedef struct _Efl_Access_Action_Data Efl_Access_Action_Data;

#include "efl_access_widget_action.eo.h"

#endif
// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#endif
//
