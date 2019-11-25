/**
 * @file soft_timer.h
 *
 * @brief Public interface for Software Timer.
 */

#ifndef __SOFT_TIMER_H__
#define __SOFT_TIMER_H__

/*****************************************************************************
 * Public constants.
 *****************************************************************************/

/**
 * @brief Maximum number of simultaneously allocated software timer instances.
 */
#define SOFT_TIMER_MAX_INSTANCES 10

/**
 * @brief Maximum timeout value in milliseconds for a software timer.
 */
#define SOFT_TIMER_MAX_RELOAD_MS 100000000

/*****************************************************************************
 * Public types.
 *****************************************************************************/

/**
 * @brief Forward declaration of software timer instance.
 */
typedef struct soft_timer soft_timer_t;

/**
 * @brief Type for timeout callbacks.
 *
 * @param p_timer Pointer to timer triggering timeout callback.
 */
typedef void (*soft_timer_callback_t)(soft_timer_t *p_timer);

/**
 * @brief Status codes for software timer functions.
 */
typedef enum soft_timer_status
{
    SOFT_TIMER_STATUS_SUCCESS = 0,       /**< Operation succeeded. */
    SOFT_TIMER_STATUS_INVALID_PARAMETER, /**< Failure: invalid parameter. */
    SOFT_TIMER_STATUS_INVALID_STATE,     /**< Failure: invalid timer state. */
} soft_timer_status_t;

/*****************************************************************************
 * Public functions.
 *****************************************************************************/

/**
 * @brief Initialize software timer module.
 */
void soft_timer_init(void);

/**
 * @brief Allocate and initialize software timer instance.
 *
 * @param pp_timer Output parameter: Pointer to newly created software timer
 *                 instance.
 */
void soft_timer_create(soft_timer_t **pp_timer);

/**
 * @brief Configure countdown timer.
 *
 * @param p_timer    Pointer to timer instance to be configured.
 * @param timeout_cb Pointer to timeout callback function.
 * @param reload_ms  Value to reload timer in milliseconds.
 * @param repeat     Boolean flag signalling if timer should repeat after
 *                   timeout.
 *
 * @return Operation status. Check @ref soft_timer_status_t.
 */
soft_timer_status_t soft_timer_set(soft_timer_t          *p_timer,
                                   soft_timer_callback_t  timeout_cb,
                                   uint32_t               reload_ms,
                                   bool                   repeat);

/**
 * @brief Start timer.
 *
 * @param p_timer    Pointer to timer instance to be started.
 *
 * @return Operation status. Check @ref soft_timer_status_t.
 */
soft_timer_status_t soft_timer_start(soft_timer_t *p_timer);

/**
 * @brief Stop timer.
 *
 * @param p_timer    Pointer to timer instance to be started.
 *
 * @return Operation status. Check @ref soft_timer_status_t.
 */
soft_timer_status_t soft_timer_stop(soft_timer_t *p_timer);

/**
 * @brief Deallocate software timer instance.
 *
 * @param pp_timer Input / Output parameter: Pointer to destroyed software
 *                 timer instance.
 */
void soft_timer_destroy(soft_timer_t **pp_timer);

#endif /** __SOFT_TIMER_H__ */
