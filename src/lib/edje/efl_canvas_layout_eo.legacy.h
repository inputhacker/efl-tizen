#ifndef _EFL_CANVAS_LAYOUT_EO_LEGACY_H_
#define _EFL_CANVAS_LAYOUT_EO_LEGACY_H_

#ifndef _EFL_CANVAS_LAYOUT_EO_CLASS_TYPE
#define _EFL_CANVAS_LAYOUT_EO_CLASS_TYPE

typedef Eo Efl_Canvas_Layout;

#endif

#ifndef _EFL_CANVAS_LAYOUT_EO_TYPES
#define _EFL_CANVAS_LAYOUT_EO_TYPES


#endif

/**
 * @brief Whether this object is animating or not.
 *
 * This property indicates whether animations are stopped or not. Animations
 * here refer to transitions between states.
 *
 * If animations are disabled, transitions between states (as defined in EDC)
 * are then instantaneous. This is conceptually similar to setting the
 * @ref Efl.Player.play_speed to an infinitely high value.
 *
 * Start or stop animating this object.
 *
 * @param[in] obj The object.
 * @param[in] on The animation state, @c true by default.
 *
 * @ingroup Edje_Object_Group
 */
EAPI void edje_object_animation_set(Efl_Canvas_Layout *obj, Eina_Bool on);

/**
 * @brief Whether this object is animating or not.
 *
 * This property indicates whether animations are stopped or not. Animations
 * here refer to transitions between states.
 *
 * If animations are disabled, transitions between states (as defined in EDC)
 * are then instantaneous. This is conceptually similar to setting the
 * @ref Efl.Player.play_speed to an infinitely high value.
 *
 * Get the current state of animation, @c true by default.
 *
 * @param[in] obj The object.
 *
 * @return The animation state, @c true by default.
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_animation_get(const Efl_Canvas_Layout *obj);

/**
 * @brief Returns the seat device given its Edje's name.
 *
 * Edje references seats by a name that differs from Evas. Edje naming follows
 * a incrementional convention: first registered name is "seat1", second is
 * "seat2", differently from Evas.
 *
 * @param[in] obj The object.
 * @param[in] name The name's character string.
 *
 * @return The seat device or @c null if not found.
 *
 * @since 1.19
 *
 * @ingroup Edje_Object_Group
 */
EAPI Efl_Input_Device *edje_object_seat_get(const Efl_Canvas_Layout *obj, Eina_Stringshare *name);

/**
 * @brief Gets the name given to a set by Edje.
 *
 * Edje references seats by a name that differs from Evas. Edje naming follows
 * a incrementional convention: first registered name is "seat1", second is
 * "seat2", differently from Evas.
 *
 * @param[in] obj The object.
 * @param[in] device The seat device
 *
 * @return The name's character string or @c null if not found.
 *
 * @since 1.19
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Stringshare *edje_object_seat_name_get(const Efl_Canvas_Layout *obj, Efl_Input_Device *device);

/**
 * @brief Gets the (last) file loading error for a given object.
 *
 * @param[in] obj The object.
 *
 * @return The load error code.
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Error edje_object_layout_load_error_get(const Efl_Canvas_Layout *obj);

/**
 * @brief Unswallow an object from this Edje.
 *
 * @param[in] obj The object.
 * @param[in] content To be removed content.
 *
 * @return @c false if @c content was not a child or can not be removed.
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_content_remove(Efl_Canvas_Layout *obj, Efl_Gfx_Entity *content);

/***********************************************************************************
* TIZEN_ONLY_FEATURE: API for handling text properties of Edje                    *
***********************************************************************************/
/**
 * @brief Sets the object text min calculation policy.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage.
 *
 * @if MOBILE @since_tizen 3.0 @elseif WEARABLE @since_tizen 3.0 @endif
 * @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] state_name The state name
 * @param[in] min_x The min width policy
 * @param[in] min_y The min height policy
 *
 * @return @c true on success, or @c false on error
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_part_text_min_policy_set(Efl_Canvas_Layout *obj, const char *part, const char *state_name, Eina_Bool min_x, Eina_Bool min_y);

/**
 * @brief Gets the object text min calculation policy.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage.
 *
 * @if MOBILE @since_tizen 3.0 @elseif WEARABLE @since_tizen 3.0 @endif
 * @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] state_name The state name
 * @param[out] min_x The min width policy
 * @param[out] min_y The min height policy
 *
 * @return @c true on success, or @c false on error
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_part_text_min_policy_get(const Efl_Canvas_Layout *obj, const char *part, const char *state_name, Eina_Bool *min_x, Eina_Bool *min_y);

/**
 * @brief Sets the valign for text.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] valign The valign 0.0~1.0. -1.0 for respect EDC's align value.
 *
 * @return @c true, on success or @c false, on error
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_part_text_valign_set(Efl_Canvas_Layout *obj, const char *part, double valign);

/**
 * @brief Gets the valign for text.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 *
 * @return The valign 0.0~1.0. -1.0 for respect EDC's align value.
 *
 * @ingroup Edje_Object_Group
 */
EAPI double edje_object_part_text_valign_get(const Efl_Canvas_Layout *obj, const char *part);

/**
 * @brief Sets the duration for text's marquee.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] duration The duration. 0.0 for respect EDC's duration value.
 *
 * @return @c true, on success or @c false, on error
 *
 * @ingroup Edje_Object_Group
 */
/*******
* END *
*******/

/***********************************************************************************
* TIZEN_ONLY_FEATURE: ellipsize.marquee, ellipsize.fade for TEXTBLOCK, TEXT part. *
***********************************************************************************/
/**
 * @brief Sets the duration for text's marquee.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] duration The duration. 0.0 for respect EDC's duration value.
 *
 * @return @c true, on success or @c false, on error
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_part_text_marquee_duration_set(Efl_Canvas_Layout *obj, const char *part, double duration);

/**
 * @brief Gets the duration for text's marquee.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 *
 * @return The duration. 0.0 for respect EDC's duration value.
 *
 * @ingroup Edje_Object_Group
 */
EAPI double edje_object_part_text_marquee_duration_get(const Efl_Canvas_Layout *obj, const char *part);

/**
 * @brief Sets the speed for text's marquee.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] speed The speed. 0.0 for respect EDC's speed value.
 *
 * @return @c true, on success or @c false, on error
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_part_text_marquee_speed_set(Efl_Canvas_Layout *obj, const char *part, double speed);

/**
 * @brief Gets the speed for text's marquee.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 *
 * @return The speed. 0.0 for respect EDC's speed value.
 *
 * @ingroup Edje_Object_Group
 */
EAPI double edje_object_part_text_marquee_speed_get(const Efl_Canvas_Layout *obj, const char *part);

/**
 * @brief Sets the always mode for text's marquee.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] always The always mode
 *
 * @return @c true, on success or @c false, on error
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_part_text_marquee_always_set(Efl_Canvas_Layout *obj, const char *part, Eina_Bool always);

/**
 * @brief Gets the always mode for text's marquee.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 *
 * @return The always mode
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_part_text_marquee_always_get(const Efl_Canvas_Layout *obj, const char *part);
/*******
* END *
*******/
/***********************************************************************************
 * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
 ***********************************************************************************/
/**
 * @brief Sets the parent object for color class.
 *
 * @if MOBILE @since_tizen 3.0 @elseif WEARABLE @since_tizen 3.0 @endif
 * @internal
 *
 * @param[in] obj The object.
 * @param[in] parent The parent object for color class
 *
 * @ingroup Edje_Object_Group
 */
EAPI void edje_object_color_class_parent_set(Efl_Canvas_Layout *obj, Efl_Object *parent) EINA_ARG_NONNULL(2);

/**
 * @brief Unsets the parent object for color class.
 *
 * @if MOBILE @since_tizen 3.0 @elseif WEARABLE @since_tizen 3.0 @endif
 * @internal
 * @param[in] obj The object.
 *
 * @ingroup Edje_Object_Group
 */
EAPI void edje_object_color_class_parent_unset(Efl_Canvas_Layout *obj);
/*******
* END *
*******/
/***********************************************************************************
 * TIZEN_ONLY_FEATURE: API for handling common properties of Edje                  *
 ***********************************************************************************/
/**
 * @brief Sets the valign for a common description.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] valign The valign 0.0~1.0. -1.0 for respect EDC's align value.
 *
 * @return @c true, on success or @c false, on error
 *
 * @ingroup Edje_Object_Group
 */
EAPI Eina_Bool edje_object_part_valign_set(Efl_Canvas_Layout *obj, const char *part, double valign);

/**
 * @brief Gets the valign for a common description.
 *
 * Do not use this API without understanding whats going on. It is made for
 * internal usage. @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 *
 * @return The valign 0.0~1.0. -1.0 for respect EDC's align value.
 *
 * @ingroup Edje_Object_Group
 */
EAPI double edje_object_part_valign_get(const Efl_Canvas_Layout *obj, const char *part);

/*******
 * END *
 *******/
/**********************************************************************************
 * TIZEN_ONLY(20171128): add additional cursor function for improving performance *
 **********************************************************************************/
/**
 * @brief Get a position of the given cursor
 *
 * @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] cur cursor type
 * @param[out] x w
 * @param[out] y h
 *
 * @ingroup Edje_Object_Group
 */
EAPI void edje_object_part_text_cursor_coord_get(Efl_Canvas_Layout *obj, const char *part, Edje_Cursor cur, int *x, int *y);

/**
 * @brief Get a size of the given cursor
 *
 * @internal
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[in] cur cursor type
 * @param[out] w w
 * @param[out] h h
 *
 * @ingroup Edje_Object_Group
 */
EAPI void edje_object_part_text_cursor_size_get(Efl_Canvas_Layout *obj, const char *part, Edje_Cursor cur, int *w, int *h);
/*******
 * END *
 *******/
/*********************************************************************
 * TIZEN_ONLY(20161110): keep cursor position on mouse down and move *
 *********************************************************************/
/**
 * @brief Returns the cursor geometry of the part relative to the edje object.
 * The cursor geometry is kept in mouse down and move.
 *
 * @internal @if MOBILE @since_tizen 3.0 @elseif WEARABLE @since_tizen 3.0
 * @endif
 *
 * @param[in] obj The object.
 * @param[in] part The part name
 * @param[out] x Cursor X position
 * @param[out] y Cursor Y position
 * @param[out] w Cursor width
 * @param[out] h Cursor height
 *
 * @ingroup Edje_Object_Group
 */
EAPI void edje_object_part_text_cursor_on_mouse_geometry_get(const Efl_Canvas_Layout *obj, const char *part, int *x, int *y, int *w, int *h);
/*******
 * END *
 *******/


#endif
