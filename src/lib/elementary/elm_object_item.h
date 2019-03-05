/**
 * @typedef Elm_Object_Item
 * An Elementary Object item handle.
 * @ingroup Elm_General
 */
typedef Eo Elm_Object_Item;

/**
 * @typedef Elm_Object_Item_Signal_Cb
 *
 * Elm_Object_Item Signal Callback functions' prototype definition. @c data
 * will have the auxiliary data pointer at the time the callback registration.
 * @c it will be a pointer the Elm_Object_Item that have the edje object where
 * the signal comes from. @c emission will identify the exact signal's emission
 * string and @c source the exact signal's source one.
 *
 * @see elm_object_item_signal_callback_add()
 * @since 1.8
 *
 * @ingroup Elm_General
 */
typedef void                  (*Elm_Object_Item_Signal_Cb)(void *data, Elm_Object_Item *it, const char *emission, const char *source);

#ifndef EFL_NOLEGACY_API_SUPPORT
#include "elm_widget_item_eo.legacy.h"
#endif

#define elm_object_item_content_set(it, content) elm_object_item_part_content_set((it), NULL, (content))

#define elm_object_item_content_get(it) elm_object_item_part_content_get((it), NULL)

#define elm_object_item_content_unset(it) elm_object_item_part_content_unset((it), NULL)

/**
 * Macro to set a label of an object item.
 *
 * @param it The Elementary object item.
 * @param label The new text of the label.
 *
 * @note Elementary object items may have many labels.
 *
 * @ingroup Elm_General
 */
#define elm_object_item_text_set(it, label) elm_object_item_part_text_set((it), NULL, (label))

#define elm_object_item_text_get(it) elm_object_item_part_text_get((it), NULL)

#define elm_object_item_domain_translatable_text_set(it, domain, text) elm_object_item_domain_translatable_part_text_set((it), NULL, (domain), (text))

#define elm_object_item_translatable_text_set(it, text) elm_object_item_domain_translatable_part_text_set((it), NULL, NULL, (text))

#define elm_object_item_translatable_part_text_set(it, part, text) elm_object_item_domain_translatable_part_text_set((it), (part), NULL, (text))

#define elm_object_item_translatable_text_get(it) elm_object_item_translatable_part_text_get((it), NULL)

#define elm_object_item_part_text_translatable_set(it, part, translatable) elm_object_item_domain_part_text_translatable_set((it), (part), NULL, (translatable))

#define elm_object_item_domain_text_translatable_set(it, domain, translatable) elm_object_item_domain_part_text_translatable_set((it), NULL, (domain), (translatable))

#define elm_object_item_text_translatable_set(it, translatable) elm_object_item_domain_part_text_translatable_set((it), NULL, NULL, (translatable))

/**
 * Get the data associated with an object item
 * @param it The Elementary object item
 * @return The data associated with @p it
 *
 * @note Every elm_object_item supports this API
 * @ingroup Elm_General
 */
EAPI void *elm_object_item_data_get(const Elm_Object_Item *it);

/**
 * Set the data associated with an object item
 * @param it The Elementary object item
 * @param data The data to be associated with @p it
 *
 * @note Every elm_object_item supports this API
 * @ingroup Elm_General
 */
EAPI void elm_object_item_data_set(Elm_Object_Item *it, void *data);

/** Delete the given item.
 *
 * @ingroup Elm_General
 */
EAPI void elm_object_item_del(Elm_Object_Item *obj);

/***********************************************************************************
 * TIZEN_ONLY_FEATURE: apply Tizen's color_class features.                         *
 ***********************************************************************************/
/**
 * @brief Set the color of color class for a given widget item.
 *
 * @note These color values are expected to be premultiplied by @p a.
 *
 * @param[in] it The Elementary widget item.
 * @param[in] color_class The name of color class.
 * @param[in] r The red intensity of the red color.
 * @param[in] g The green intensity of the green color.
 * @param[in] b The blue intensity of the blue color.
 * @param[in] a The alpha value.
 *
 * @since 1.19
 *
 * @ingroup General
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Eina_Bool        elm_object_item_color_class_color_set(Elm_Object_Item *it, const char *color_class, int r, int g, int b, int a);

/**
 * @brief Get the color of color class for a given widget item.
 *
 * @note These color values are expected to be premultiplied by @p a.
 *
 * @param[in] it The Elementary widget item.
 * @param[in] color_class The name of color class.
 * @param[in] r The red intensity of the red color.
 * @param[in] g The green intensity of the green color.
 * @param[in] b The blue intensity of the blue color.
 * @param[in] a The alpha value.
 *
 * @since 1.19
 *
 * @ingroup General
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Eina_Bool        elm_object_item_color_class_color_get(Elm_Object_Item *it, const char *color_class, int *r, int *g, int *b, int *a);

/**
 * @brief Set the second color of color class for a given widget item.
 *
 * The second color is commonly used for outline of text.
 *
 * @note These color values are expected to be premultiplied by @p a.
 *
 * @param[in] it The Elementary widget item.
 * @param[in] color_class The name of color class.
 * @param[in] r The red intensity of the red color.
 * @param[in] g The green intensity of the green color.
 * @param[in] b The blue intensity of the blue color.
 * @param[in] a The alpha value.
 *
 * @since 1.19
 *
 * @ingroup General
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Eina_Bool        elm_object_item_color_class_color2_set(Elm_Object_Item *it, const char *color_class, int r, int g, int b, int a);

/**
 * @brief Get the second color of color class for a given widget item.
 *
 * The second color is commonly used for outline of text.
 *
 * @note These color values are expected to be premultiplied by @p a.
 *
 * @param[in] it The Elementary widget item.
 * @param[in] color_class The name of color class.
 * @param[in] r The red intensity of the red color.
 * @param[in] g The green intensity of the green color.
 * @param[in] b The blue intensity of the blue color.
 * @param[in] a The alpha value.
 *
 * @since 1.19
 *
 * @ingroup General
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Eina_Bool        elm_object_item_color_class_color2_get(Elm_Object_Item *it, const char *color_class, int *r, int *g, int *b, int *a);

/**
 * @brief Set the third color of color class for a given widget item.
 *
 * The third color is commonly used for shadow of text.
 *
 * @note These color values are expected to be premultiplied by @p a.
 *
 * @param[in] it The Elementary widget item.
 * @param[in] color_class The name of color class.
 * @param[in] r The red intensity of the red color.
 * @param[in] g The green intensity of the green color.
 * @param[in] b The blue intensity of the blue color.
 * @param[in] a The alpha value.
 *
 * @since 1.19
 *
 * @ingroup General
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Eina_Bool        elm_object_item_color_class_color3_set(Elm_Object_Item *it, const char *color_class, int r, int g, int b, int a);

/**
 * @brief Get the third color of color class for a given widget item.
 *
 * The third color is commonly used for shadow of text.
 *
 * @note These color values are expected to be premultiplied by @p a.
 *
 * @param[in] it The Elementary widget item.
 * @param[in] color_class The name of color class.
 * @param[in] r The red intensity of the red color.
 * @param[in] g The green intensity of the green color.
 * @param[in] b The blue intensity of the blue color.
 * @param[in] a The alpha value.
 *
 * @since 1.19
 *
 * @ingroup General
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Eina_Bool        elm_object_item_color_class_color3_get(Elm_Object_Item *it, const char *color_class, int *r, int *g, int *b, int *a);

/**
 * @brief Delete the color class for the Elementary widget item.
 *
 * Deleting the color class will revert it to the values defined by
 * elm_color_class_set() or the theme file.
 *
 * @param[in] it The Elementary widget item.
 * @param[in] color_class The name of color class.
 *
 * @ingroup Elm_General
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void             elm_object_item_color_class_del(Elm_Object_Item *it, const char *color_class);

/**
 * @brief Delete all color classes for the Elementary widget item.
 *
 * This function deletes any color classes defined for the Elementary widget item.
 * Clearing color classes will revert the color of all parts to the values
 * defined by elm_color_class_set() or the theme file.]]
 *
 * @param[in] it The Elementary widget item.
 *
 * @ingroup Elm_General
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void             elm_object_item_color_class_clear(Elm_Object_Item *it);
/*******
 * END *
 *******/
