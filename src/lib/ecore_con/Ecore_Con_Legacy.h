#ifndef _ECORE_CON_LEGACY_H
#define _ECORE_CON_LEGACY_H
#include <Eina.h>
#include <Eo.h>

#include "ecore_con_base.eo.legacy.h"
#include "ecore_con_server.eo.legacy.h"
#include "ecore_con_connector.eo.legacy.h"
#include "ecore_con_client.eo.legacy.h"


/********************************************************************
 * ecore_con_url.eo.h
 *******************************************************************/
/**
 * @ingroup Ecore_Con_Url_Group
 * @typedef Ecore_Con_Url
 * Used to provide legacy API/ABI compatibility with non-Eo applications.
 *
 * @if MOBILE @since_tizen 3.0
 * @else WEARABLE @since_tizen 3.0
 * @endif
 */
typedef Eo Ecore_Con_Url;


/********************************************************************
 * ecore_con_url.eo.legacy.h
 *******************************************************************/
/**
 * @ingroup Ecore_Con_Url_Group
 * @brief Controls the URL to send the request to.
 * @param[in] obj The Given object
 * @param[in] url the new URL
 * @return #EINA_TRUE on success, #EINA_FALSE otherwise.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Eina_Bool ecore_con_url_url_set(Ecore_Con_Url *obj, const char *url);

/**
 * @ingroup Ecore_Con_Url_Group
 * @brief Controls the URL to send the request to.
 * @param[in] obj The given object
 * @return @c NULL on error, read-only URL string on success.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI const char *ecore_con_url_url_get(const Ecore_Con_Url *obj);

#endif
