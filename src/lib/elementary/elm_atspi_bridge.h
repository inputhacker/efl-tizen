#ifndef EFL_NOLEGACY_API_SUPPORT
#include "elm_atspi_bridge_eo.legacy.h"
#endif

/**
 * @defgroup Elm_Atspi_Bridge Atspi Bridge
 * @ingroup elm_accessibility_group
 */
//TIZEN_ONLY(20160527) - Add direct reading feature
/**
 * This callback type is used to provide reading status information.
 * @param data The data specified as the last param when setting this callback
 * @param say_signal The signal describes reading status information
 * @see elm_atspi_bridge_utils_say
 * @ingroup Elm_Atspi_Bridge
 * @if MOBILE @since_tizen 4.0 @elseif WEARABLE @since_tizen 3.0 @endif
 */
typedef void (*Elm_Atspi_Say_Signal_Cb)(void *data, const char *say_signal);
/**
 * @brief Reads given text by screen reader
 *
 * @param text The text to read
 * @param discardable If TRUE, reading can be discarded by subsequent reading requests,
 * if FALSE the reading must finish before next reading request can be started
 * @param func the callback function that is called on reading signals emitted
 * during processing of this reading request. Elm_Atspi_Say_Signal_Cb callback
 * type specifies two parameters (void *data, const char *signal), where data
 * is a user data and signal can be one of the following signals:
 * “ReadingCancelled”
 * “ReadingStopped”
 * “ReadingSkipped”
 * @param data The user data to be passed to the callback function
 *
 * @ingroup Elm_Atspi_Bridge
 *
 * @if MOBILE @since_tizen 4.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void elm_atspi_bridge_utils_say(const char* text,
                                     Eina_Bool discardable,
                                     const Elm_Atspi_Say_Signal_Cb func,
                                     const void *data);
//
//TIZEN_ONLY(20161027) - Export elm_atspi_bridge_utils_is_screen_reader_enabled
/**
 * @internal
 * Determines if screen reader is enabled
 * @ingroup Elm_Atspi_Bridge
 */
EAPI Eina_Bool elm_atspi_bridge_utils_is_screen_reader_enabled(void);
//
