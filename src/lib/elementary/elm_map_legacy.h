
/**
 * @brief Set map's zoom behavior. It can be set to manual or automatic.
 *
 * Default value is #ELM_MAP_ZOOM_MODE_MANUAL.
 *
 * Values <b> don't </b> work as bitmask, only one can be chosen.
 *
 * Valid sizes are 2^zoom, consequently the map may be smaller than the
 * scroller view.
 *
 * See @ref elm_map_zoom_mode_set() See @ref elm_map_zoom_mode_get()
 *
 * @ingroup Elm_Map
 */
typedef enum
{
  ELM_MAP_ZOOM_MODE_MANUAL = 0, /**< Zoom controlled manually by
                                 * elm_map_zoom_set(). It's set by default. */
  ELM_MAP_ZOOM_MODE_AUTO_FIT, /**< Zoom until map fits inside the scroll frame
                               * with no pixels outside this area. */
  ELM_MAP_ZOOM_MODE_AUTO_FILL, /**< Zoom until map fills scroll, ensuring no
                                * pixels are left unfilled. */
  ELM_MAP_ZOOM_MODE_LAST /**< Sentinel value to indicate last enum field during
                          * iteration */
} Elm_Map_Zoom_Mode;

/**
 * Add a new map widget to the given parent Elementary (container) object.
 *
 * @param parent The parent object.
 * @return a new map widget handle or @c NULL, on errors.
 *
 * This function inserts a new map widget on the canvas.
 *
 * @ingroup Elm_Map
 */
EAPI Evas_Object          *elm_map_add(Evas_Object *parent);


/**
 * @brief Set the zoom level of the map.
 *
 * This sets the zoom level.
 *
 * It will respect limits defined by @ref elm_map_zoom_min_set and
 * @ref elm_map_zoom_max_set.
 *
 * By default these values are 0 (world map) and 18 (maximum zoom).
 *
 * This function should be used when zoom mode is set to
 * #ELM_MAP_ZOOM_MODE_MANUAL. This is the default mode, and can be set with
 * @ref elm_map_zoom_mode_set.
 *
 * @param[in] obj The object.
 * @param[in] zoom The zoom level.
 *
 * @ingroup Elm_Map
 */
EAPI void elm_map_zoom_set(Evas_Object *obj, int zoom);

/**
 * @brief Get the zoom level of the map.
 *
 * This returns the current zoom level of the map object.
 *
 * Note that if you set the fill mode to other than #ELM_MAP_ZOOM_MODE_MANUAL
 * (which is the default), the zoom level may be changed at any time by the map
 * object itself to account for map size and map viewport size.
 *
 * @param[in] obj The object.
 *
 * @return The zoom level.
 *
 * @ingroup Elm_Map
 */
EAPI int elm_map_zoom_get(const Evas_Object *obj);


/**
 * @brief Set the zoom mode used by the map object.
 *
 * This sets the zoom mode to manual or one of the automatic levels. Manual
 * (#ELM_MAP_ZOOM_MODE_MANUAL) means that zoom is set manually by
 * @ref elm_map_zoom_mode_set and will stay at that level until changed by code
 * or until zoom mode is changed. This is the default mode.
 *
 * The Automatic modes will allow the map object to automatically adjust zoom
 * mode based on properties. #ELM_MAP_ZOOM_MODE_AUTO_FIT will adjust zoom so
 * the map fits inside the scroll frame with no pixels outside this area.
 * #ELM_MAP_ZOOM_MODE_AUTO_FILL will be similar but ensure no pixels within the
 * frame are left unfilled. Do not forget that the valid sizes are 2^zoom,
 * consequently the map may be smaller than the scroller view.
 *
 * @param[in] obj The object.
 * @param[in] mode The zoom mode of the map, being it one of
 * #ELM_MAP_ZOOM_MODE_MANUAL (default), #ELM_MAP_ZOOM_MODE_AUTO_FIT, or
 * #ELM_MAP_ZOOM_MODE_AUTO_FILL.
 *
 * @ingroup Elm_Map
 */
EAPI void elm_map_zoom_mode_set(Evas_Object *obj, Elm_Map_Zoom_Mode mode);

/**
 * @brief Get the zoom mode used by the map object.
 *
 * This function returns the current zoom mode used by the ma object.
 *
 * @param[in] obj The object.
 *
 * @return The zoom mode of the map, being it one of #ELM_MAP_ZOOM_MODE_MANUAL
 * (default), #ELM_MAP_ZOOM_MODE_AUTO_FIT, or #ELM_MAP_ZOOM_MODE_AUTO_FILL.
 *
 * @ingroup Elm_Map
 */
EAPI Elm_Map_Zoom_Mode elm_map_zoom_mode_get(const Evas_Object *obj);

#include "elm_map.eo.legacy.h"
