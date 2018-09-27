#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup Ecore_Poller_Group
 *
 * @{
 */

#include "ecore_poller.eo.legacy.h"

/**
 * @brief Creates a poller to call the given function at a particular tick interval.
 * @param type The ticker type to attach the poller to. Must be ECORE_POLLER_CORE.
 * @param interval The poll interval.
 * @param func The poller function.
 * @param data Data to pass to @a func when it is called.
 * @return A poller object on success, @c NULL otherwise.
 *
 * This function adds @a func as a poller callback that will be called every @a
 * interval ticks together with other pollers of type @a type. @a func will be
 * passed the @p data pointer as a parameter.
 *
 * The @p interval must be between 1 and 32768 inclusive, and must be a power of
 * 2 (i.e. 1, 2, 4, 8, 16, ... 16384, 32768). The exact tick in which @a func
 * will be called is undefined, as only the interval between calls can be
 * defined. Ecore will endeavor to keep pollers synchronized and to call as
 * many in 1 wakeup event as possible. If @a interval is not a power of two, the
 * closest power of 2 greater than @a interval will be used.
 *
 * When the poller @p func is called, it must return a value of either
 * @c ECORE_CALLBACK_RENEW(or @c 1) or @c ECORE_CALLBACK_CANCEL(or @c 0). If it
 * returns @c 1, it will be called again at the next tick, or if it returns
 * @c 0 it will be deleted automatically making any references/handles for it
 * invalid.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Poller *ecore_poller_add(Ecore_Poller_Type type, int interval, Ecore_Task_Cb func, const void *data);

/**
 * @brief Deletes the specified poller from the timer list.
 * @param poller The poller to delete.
 * @return The data pointer set for the timer when @ref ecore_poller_add was
 * called on success, @c NULL otherwise.
 *
 * @note @a poller must be a valid handle. If the poller function has already
 * returned @c 0, the handle is no longer valid (and does not need to be deleted).
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void *ecore_poller_del(Ecore_Poller *poller);



/**
 * @}
 */

/**
 * @addtogroup Ecore_Animator_Group
 *
 * @{
 */

/**
 * @brief Adds an animator to call @p func at every animation tick during main
 * loop execution.
 *
 * @param func The function to call when it ticks off
 * @param data The data to pass to the function
 * @return A handle to the new animator
 *
 * This function adds an animator and returns its handle on success and @c NULL
 * on failure. The function @p func will be called every N seconds where N is
 * the @p frametime interval set by ecore_animator_frametime_set(). The
 * function will be passed the @p data pointer as its parameter.
 *
 * When the animator @p func is called, it must return a boolean value.
 * If it returns @c EINA_TRUE (or @c ECORE_CALLBACK_RENEW), it will be called again at
 * the next tick, or if it returns @c EINA_FALSE (or @c ECORE_CALLBACK_CANCEL) it will be
 * deleted automatically making any references/handles for it invalid.
 * @see ecore_animator_timeline_add()
 * @see ecore_animator_frametime_set()
 *
 * @note The default @p frametime value is 1/30th of a second.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Animator *ecore_animator_add(Ecore_Task_Cb func, const void *data);

/**
 * @brief Adds an animator that runs for a limited time.
 *
 * @param runtime The time to run in seconds
 * @param func The function to call when it ticks off
 * @param data The data to pass to the function
 * @return A handle to the new animator
 *
 * This function is just like ecore_animator_add() except the animator only
 * runs for a limited time specified in seconds by @p runtime. Once the
 * runtime the animator has elapsed (animator finished) it will automatically
 * be deleted. The callback function @p func can return @c ECORE_CALLBACK_RENEW
 * to keep the animator running or @c ECORE_CALLBACK_CANCEL ro stop it and have
 * it be deleted automatically at any time.
 *
 * The @p func will ALSO be passed a position parameter that will be in value
 * from 0.0 to 1.0 to indicate where along the timeline (0.0 start, 1.0 end)
 * the animator run is at. If the callback wishes not to have a linear
 * transition it can "map" this value to one of several curves and mappings
 * via ecore_animator_pos_map().
 * @see ecore_animator_add()
 * @see ecore_animator_pos_map()
 *
 * @note The default @p frametime value is 1/30th of a second.
 *
 * @since 1.1.0
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Animator *ecore_animator_timeline_add(double runtime, Ecore_Timeline_Cb func, const void *data);

/**
 * @brief Deletes the specified animator from the animator list.
 *
 * @param animator The animator to delete
 * @return The data pointer set for the animator on add
 *
 * Deletes the specified @p animator from the set of animators that are
 * executed during main loop execution. This function returns the data
 * parameter that was being passed to the callback on success, or @c NULL on
 * failure. After this call returns the specified animator object @p animator
 * is invalid and should not be used again. It will not get called again after
 * deletion.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void *ecore_animator_del(Ecore_Animator *animator);

/**
 * @brief Suspends the specified animator.
 *
 * @param animator The animator to delete
 *
 * The specified @p animator will be temporarily removed from the set of
 * animators that are executed during main loop.
 *
 * @warning Freezing an animator doesn't freeze accounting of how long that
 * animator has been running. Therefore if the animator was created with
 *ecore_animator_timeline_add() the @p pos argument given to the callback
 * will increase as if the animator hadn't been frozen and the animator may
 * have it's execution halted if @p runtime elapsed.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void ecore_animator_freeze(Ecore_Animator *animator);

/**
 * @brief Restores execution of the specified animator.
 *
 * @param animator The animator to delete
 *
 * The specified @p animator will be put back in the set of animators that are
 * executed during main loop.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void ecore_animator_thaw(Ecore_Animator *animator);

#include "ecore_animator.eo.legacy.h"

/**
 * @}
 */

/**
 * @addtogroup Ecore_Timer_Group
 *
 * @{
 */

/**
 * Creates a timer to call the given function in the given period of time.
 * @param   in   The interval in seconds.
 * @param   func The given function.  If @p func returns @c 1, the timer is
 *               rescheduled for the next interval @p in.
 * @param   data Data to pass to @p func when it is called.
 * @return  A timer object on success,  @c NULL on failure.
 *
 * This function adds a timer and returns its handle on success and @c NULL on
 * failure. The function @p func will be called every @p in seconds. The
 * function will be passed the @p data pointer as its parameter.
 *
 * When the timer @p func is called, it must return a value of either @c 1
 * (or @c ECORE_CALLBACK_RENEW) or @c 0 (or @c ECORE_CALLBACK_CANCEL).
 * If it returns @c 1, it will be called again at the next tick, or if it returns
 * @c 0 it will be deleted automatically making any references/handles for it
 * invalid.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Timer *ecore_timer_add(double in, Ecore_Task_Cb func, const void *data);

/**
 * Creates a timer to call the given function in the given period of time.
 * @param   in   The interval in seconds from current loop time.
 * @param   func The given function.  If @p func returns 1, the timer is
 *               rescheduled for the next interval @p in.
 * @param   data Data to pass to @p func when it is called.
 * @return  A timer object on success,  @c NULL on failure.
 *
 * This is the same as ecore_timer_add(), but "now" is the time from
 * ecore_loop_time_get() not ecore_time_get() as ecore_timer_add() uses. See
 * ecore_timer_add() for more details.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Timer *ecore_timer_loop_add(double in, Ecore_Task_Cb func, const void *data);

/**
 * Deletes the specified timer from the timer list.
 * @param   timer The timer to delete.
 * @return  The data pointer set for the timer when @ref ecore_timer_add was
 *          called.  @c NULL is returned if the function is unsuccessful.
 *
 * Note: @p timer must be a valid handle. If the timer function has already
 * returned @c 0, the handle is no longer valid (and does not need to be delete).
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void *ecore_timer_del(Ecore_Timer *timer);

/**
 * @brief Pauses a running timer.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 *
 * @remarks The timer callback won't be called while the timer is paused. The remaining
 *          time until the timer expires will be saved, so the timer can be resumed with
 *          that same remaining time to expire, instead of expiring instantly.  Use
 *          ecore_timer_thaw() to resume it.
 *
 * @remarks Nothing happens if the timer was already paused.
 *
 * @param[in] timer The timer to be paused.
 *
 * @see ecore_timer_thaw()
 */
EAPI void ecore_timer_freeze(Ecore_Timer *timer);

/**
 * @brief Return whether the timer is freezing.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 *
 * @return True if the timer object is freezed, false otherwise.
 *
 * @see ecore_timer_freeze(), ecore_timer_thaw()
 */
EAPI Eina_Bool ecore_timer_freeze_get(Ecore_Timer *timer);

/**
 * @brief Resumes a frozen (paused) timer.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 *
 * @remarks The timer will be resumed from its previous relative position in time. That
 *          means, if it had X seconds remaining until expire when it was paused, it will
 *          be started now with those same X seconds remaining to expire again. But
 *          notice that the interval time won't be touched by this call or by
 *          ecore_timer_freeze().
 *
 * @param[in] timer The timer to be resumed.
 *
 * @see ecore_timer_freeze()
 */
EAPI void ecore_timer_thaw(Ecore_Timer *timer);

#include "ecore_timer.eo.legacy.h"

/**
 * @}
 */

/**
 * @addtogroup Ecore_Idle_Group
 *
 * @{
 */
/**
 * Adds an idler handler.
 * @param  func The function to call when idling.
 * @param  data The data to be passed to this @p func call.
 * @return A idler handle if successfully added, @c NULL otherwise.
 *
 * Add an idler handle to the event loop, returning a handle on
 * success and @c NULL otherwise. The function @p func will be called
 * repeatedly while no other events are ready to be processed, as
 * long as it returns @c 1 (or @c ECORE_CALLBACK_RENEW). A return of @c 0
 * (or @c ECORE_CALLBACK_CANCEL) deletes the idler.
 *
 * Idlers are useful for progressively processing data without blocking.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Idler *ecore_idler_add(Ecore_Task_Cb func, const void *data);

/**
 * Deletes an idler callback from the list to be executed.
 * @param  idler The handle of the idler callback to delete
 * @return The data pointer passed to the idler callback on success, @c NULL
 *         otherwise.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void *ecore_idler_del(Ecore_Idler *idler);

/**
 * Adds an idle enterer handler.
 * @param   func The function to call when entering an idle state.
 * @param   data The data to be passed to the @p func call
 * @return  A handle to the idle enterer callback if successful.  Otherwise,
 *          @c NULL is returned.
 * @note The function func will be called every time the main loop is entering
 * idle state, as long as it returns @c 1 (or @c ECORE_CALLBACK_RENEW). A return of @c 0
 * (or @c ECORE_CALLBACK_CANCEL) deletes the idle enterer.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Idle_Enterer *ecore_idle_enterer_add(Ecore_Task_Cb func, const void *data);

/**
 * Adds an idle enterer handler at the start of the list so it gets called earlier than others.
 * @param   func The function to call when entering an idle state.
 * @param   data The data to be passed to the @p func call
 * @return  A handle to the idle enterer callback if successful.  Otherwise,
 *          @c NULL is returned.
 * @note The function func will be called every time the main loop is entering
 * idle state, as long as it returns @c 1 (or @c ECORE_CALLBACK_RENEW). A return of @c 0
 * (or @c ECORE_CALLBACK_CANCEL) deletes the idle enterer.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Idle_Enterer *ecore_idle_enterer_before_add(Ecore_Task_Cb func, const void *data);

/**
 * Deletes an idle enterer callback.
 * @param   idle_enterer The idle enterer to delete
 * @return  The data pointer passed to the idler enterer callback on success.
 *          @c NULL otherwise.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void *ecore_idle_enterer_del(Ecore_Idle_Enterer *idle_enterer);

/**
 * Adds an idle exiter handler.
 * @param func The function to call when exiting an idle state.
 * @param data The data to be passed to the @p func call.
 * @return A handle to the idle exiter callback on success.  @c NULL otherwise.
 * @note The function func will be called every time the main loop is exiting
 * idle state, as long as it returns @c 1 (or @c ECORE_CALLBACK_RENEW). A return of @c 0
 * (or @c ECORE_CALLBACK_CANCEL) deletes the idle exiter.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Idle_Exiter *ecore_idle_exiter_add(Ecore_Task_Cb func, const void *data);

/**
 * Deletes an idle exiter handler from the list to be run on exiting idle state.
 * @param idle_exiter The idle exiter to delete
 * @return The data pointer that was being being passed to the handler if
 *         successful. @c NULL otherwise.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void *ecore_idle_exiter_del(Ecore_Idle_Exiter *idle_exiter);

#include "ecore_idler.eo.legacy.h"

/**
 * @}
 */

/**
 * @addtogroup Ecore_Exe_Group
 *
 * @{
 */

#include "ecore_exe.eo.legacy.h"

/**
 * @}
 */

/**
 * @addtogroup Ecore_Job_Group
 *
 * @{
 */
/**
 * Adds a job to the event queue.
 * @param   func The function to call when the job gets handled.
 * @param   data Data pointer to be passed to the job function when the job is
 *               handled.
 * @return  The handle of the job.  @c NULL is returned if the job could not be
 *          added to the queue.
 * @note    Once the job has been executed, the job handle is invalid.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI Ecore_Job *ecore_job_add(Ecore_Cb func, const void *data);

/**
 * Deletes a queued job that has not yet been executed.
 * @param   obj  Handle of the job to delete.
 * @return  The data pointer that was to be passed to the job.
 *
 * @if MOBILE @since_tizen 2.3
 * @elseif WEARABLE @since_tizen 2.3.1
 * @endif
 */
EAPI void *ecore_job_del(Ecore_Job *obj);

#include "ecore_job.eo.legacy.h"

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
