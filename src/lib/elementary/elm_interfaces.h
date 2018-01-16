#include "efl_access.h"
#include "efl_access_text.h"
#include "efl_access_widget_action.h"
#include "efl_access_window.h"

// FIXME FIXME FIXME -- Still required because of photocam
#ifdef EFL_EO_API_SUPPORT
# include "elm_interface_scrollable.h"
#endif

// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#ifdef EFL_BETA_API_SUPPORT
//
#ifdef EFL_EO_API_SUPPORT
#include "efl_access_action.eo.h"
#include "efl_access_component.eo.h"
#include "efl_access_editable_text.eo.h"
#include "efl_access_image.eo.h"
#include "efl_access_selection.eo.h"
#include "efl_access_value.eo.h"
#endif
// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#endif
//
#ifndef EFL_NOLEGACY_API_SUPPORT
// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#ifdef EFL_BETA_API_SUPPORT
//
#include "efl_access_action.eo.legacy.h"
/**
 * @defgroup Elm_Interface_Atspi_Component Atspi Component
 * @ingroup elm_accessibility_group
 * @{
 */
#include "efl_access_component.eo.legacy.h"
/**
 * @}
 */
#include "efl_access_editable_text.eo.legacy.h"
#include "efl_access_image.eo.legacy.h"
#include "efl_access_selection.eo.legacy.h"
#include "efl_access_value.eo.legacy.h"
#endif
// TIZEN_ONLY(20171114): Add accessibility highlight for toolbar items I7892c9433d675d9ba6618991ae46488060752c3c
//#endif
//
