/**
 * Add a new genlist widget to the given parent Elementary
 * (container) object
 *
 * @param parent The parent object
 * @return a new genlist widget handle or @c NULL, on errors
 *
 * This function inserts a new genlist widget on the canvas.
 *
 * @see elm_genlist_item_append()
 * @see elm_object_item_del()
 * @see elm_genlist_clear()
 *
 * @ingroup Elm_Genlist
 */
EAPI Evas_Object                  *elm_genlist_add(Evas_Object *parent);

/**
 * Get the nth item, in a given genlist widget, placed at position @p nth, in
 * its internal items list
 *
 * @param obj The genlist object
 * @param nth The number of the item to grab (0 being the first)
 *
 * @return The item stored in @p obj at position @p nth or @c NULL, if there's
 * no item with that index (and on errors)
 *
 * @ingroup Genilst
 * @since 1.8
 */
EAPI Elm_Object_Item *
elm_genlist_nth_item_get(const Evas_Object *obj, unsigned int nth);


/**
 * @internal
 * @remarks Mobile only feature
 *
 * @brief Sets the genlist realization mode.
 *
 * @remarks By default, genlist disables the realization mode and genlists realize and
 *          unrealize some items when needed. If the realization mode is on,
 *          all items are realized when genlist is created and no items are unrealized.
 *          If this mode is on and the content size is changed, the item size
 *          changes accordingly. By default, genlist does not change item sizes
 *          eventhough they can be changed for performance reasons.
 *          This consumes more memory and decrease performance. So if the application
 *          appends many items, do not use the realization mode.
 *
 * @param obj The genlist object
 * @param mode The realization mode
 *             (@c EINA_TRUE = on, @c EINA_FALSE = off)
 */
EAPI void
elm_genlist_realization_mode_set(Evas_Object *obj, Eina_Bool mode);

/**
 * @internal
 * @remarks Mobile only feature
 *
 * @brief Gets the genlist realization mode.
 *
 * @param obj The genlist object
 * @return The realization mode
 *         (@c EINA_TRUE = on, @c EINA_FALSE = off)
 */
EAPI Eina_Bool
elm_genlist_realization_mode_get(Evas_Object *obj);

/**
 * @internal
 * @remarks Mobile only feature
 *
 * @brief Starts reordering for a specific item. it moves by move event.
 *
 * @param item The genlist item object
 */
EAPI void
elm_genlist_item_reorder_start(Elm_Object_Item *item);

/**
 * @internal
 * @remarks Mobile only feature
 *
 * @brief Stop reordering and relocate the item at touch released position.
 * @param item The genlist item object
 */
EAPI void
elm_genlist_item_reorder_stop(Elm_Object_Item *item);
/**
 * @}
 */

#include "elm_genlist_item_eo.legacy.h"
#include "elm_genlist_eo.legacy.h"
