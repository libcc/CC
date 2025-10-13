#ifndef _C_CC_EVENT_C_H_INCLUDED_
#define _C_CC_EVENT_C_H_INCLUDED_

#include <libcc/event.h>
#include <libcc/OpenSSL.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define _CC_MAX_CHANGE_EVENTS_                64

#define _CC_EVENT_IS_SOCKET(flags)                                                                                     \
    _CC_ISSET_BIT(_CC_EVENT_READABLE_ | _CC_EVENT_WRITABLE_ | _CC_EVENT_ACCEPT_ | _CC_EVENT_CONNECT_, (flags))

#ifdef _CC_EVENT_USE_MUTEX_
#define _event_lock(x)               \
    do {                                \
        if (_cc_likely((x)->lock)) {   \
            _cc_mutex_lock((x)->lock); \
        }                               \
    } while (0)

#define _event_unlock(x)               \
    do {                                  \
        if (_cc_likely((x)->lock)) {     \
            _cc_mutex_unlock((x)->lock); \
        }                                 \
    } while (0)
#else
#define _event_lock(x) _cc_spin_lock(&((x)->lock))
#define _event_unlock(x) _cc_unlock(&((x)->lock))
#endif

/*
 * @brief Initializes an event async class
 *
 * @param async _cc_async_event_t structure
 *
 * @return true if successful or false on error.
 */
bool_t _register_async_event(_cc_async_event_t *async);

/*
 * @brief Free an event async class
 *
 * @param async _cc_async_event_t structure
 *
 * @return true if successful or false on error.
 */
bool_t _unregister_async_event(_cc_async_event_t *async);

/**
 * @brief Calls back an event function
 *
 * @param async _cc_async_event_t structure
 * @param e _cc_event_t structure
 * @param which The state of an event
 *
 * @return true if successful or false on error.
 */
bool_t _event_callback(_cc_async_event_t *async, _cc_event_t *e, uint32_t which);
/**
 * @brief Check the Socket is valid
 *
 * @param fd socket
 *
 * @return true if successful or false on error.
 */
bool_t _valid_fd(_cc_socket_t fd);

/**
 * @brief Socket disconnect event
 *
 * @param async _cc_async_event_t structure
 * @param e _cc_event_t structure
 *
 * @return true if successful or false on error.
 */
bool_t _disconnect_event(_cc_async_event_t *async, _cc_event_t *e);

/**
 * @brief Wait for reset event
 *
 * @param async _cc_async_event_t structure
 * @param e _cc_event_t structure
 *
 * @return true if successful or false on error.
 */
bool_t _reset_event(_cc_async_event_t *async, _cc_event_t *e);

/**
 * @brief Reset pending events
 *
 * @param async _cc_async_event_t structure
 * @param func callback
 *
 */
void _reset_event_pending(_cc_async_event_t *async, void (*func)(_cc_async_event_t *, _cc_event_t *));
/**
 * @brief add timeout events
 *
 * @param async _cc_async_event_t structure
 * @param e _cc_event_t structure
 *
 */
void _add_event_timeout(_cc_async_event_t *async, _cc_event_t *e);
/**
 * @brief Reset timeout events
 *
 * @param async _cc_async_event_t structure
 * @param e _cc_event_t structure
 *
 */
void _reset_event_timeout(_cc_async_event_t *async, _cc_event_t *e);
/**
 * @brief Run timeout events
 *
 * @param async _cc_async_event_t structure
 * @param func callback function
 *
 * @return true if successful or false on error.
 */
void _update_event_timeout(_cc_async_event_t *async, uint32_t timeout);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_EVENT_C_H_INCLUDED_*/
