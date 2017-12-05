#ifdef EFL_BETA_API_SUPPORT
#ifdef EFL_EO_API_SUPPORT
#include "elm_atspi_bridge.eo.h"
#endif
#ifndef EFL_NOLEGACY_API_SUPPORT
#include "elm_atspi_bridge.eo.legacy.h"
#endif
#endif
//TIZEN_ONLY(20160527) - Add direct reading feature
typedef void (*Elm_Atspi_Say_Signal_Cb)(void *data, const char *say_signal);
EAPI void elm_atspi_bridge_utils_say(const char* text,
                                     Eina_Bool discardable,
                                     const Elm_Atspi_Say_Signal_Cb func,
                                     const void *data);
//
//TIZEN_ONLY(20161027) - Export elm_atspi_bridge_utils_is_screen_reader_enabled
/** Determines if screen reader is enabled
 *
 * @ingroup Elm_Atspi_Bridge
 */
EAPI Eina_Bool elm_atspi_bridge_utils_is_screen_reader_enabled(void);
//
