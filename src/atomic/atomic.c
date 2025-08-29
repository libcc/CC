/*
 * Copyright libcc.cn@gmail.com. and other libcc contributors.
 * All rights reserved.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:

 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/
#include <libcc/atomic.h>

_CC_API_PUBLIC(int32_t) _cc_atomic32_load(_cc_atomic32_t *a) {
#if __STDC_VERSION__ >= 201000L
    return (int32_t)atomic_load(a);
#elif defined(_CC_GNUC_)
    return __atomic_load_n(a, __ATOMIC_SEQ_CST);
#else
    int32_t value;
    do {
        value = (int32_t)*a;
    } while (!_cc_atomic32_cas(a, value, value));
    return value;
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int64_t) _cc_atomic64_load(_cc_atomic64_t *a) {
#if __STDC_VERSION__ >= 201000L
    return (int64_t)atomic_load(a);
#elif defined(_CC_GNUC_)
    return __atomic_load_n(a, __ATOMIC_SEQ_CST);
#else
    int64_t value;
    do {
        value = (int64_t)*a;
    } while (!_cc_atomic64_cas(a, value, value));
    return value;
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int32_t) _cc_atomic32_add(_cc_atomic32_t *a, int32_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_add(a, v);
#elif defined(_CC_MSVC_)
#if (defined(_M_IA64) || defined(_M_AMD64))
    return (int32_t)InterlockedExchangeAdd(a, v);
#else
    return (int32_t)InterlockedExchangeAdd((long *)a, v);
#endif
#elif defined(_CC_GNUC_)
    return __atomic_fetch_add(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_add(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int32_t) _cc_atomic32_sub(_cc_atomic32_t *a, int32_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_sub(a, v);
#elif defined(_CC_MSVC_)
#if (defined(_M_IA64) || defined(_M_AMD64))
    return (int32_t)InterlockedExchangeAdd(a, -v);
#else
    return (int32_t)InterlockedExchangeAdd((long *)a, -v);
#endif
#elif defined(_CC_GNUC_)
    return __atomic_fetch_sub(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_sub(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int32_t) _cc_atomic32_set(_cc_atomic32_t *a, int32_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_exchange(a, v);
#elif defined(_CC_MSVC_)
#if (defined(_M_IA64) || defined(_M_AMD64)) && !defined(RC_INVOKED)
    return (int32_t)InterlockedExchange(a, v);
#else
    return (int32_t)InterlockedExchange((long *)a, v);
#endif
#elif defined(_CC_GNUC_)
    return __atomic_exchange_n(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_lock_test_and_set(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int32_t) _cc_atomic32_and(_cc_atomic32_t *a, int32_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_and(a, v);
#elif defined(_CC_MSVC_)
    return (int32_t)_InterlockedAnd(a, v);
#elif defined(_CC_GNUC_)
    return __atomic_fetch_and(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_and_and_fetch(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int32_t) _cc_atomic32_or(_cc_atomic32_t *a, int32_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_or(a, v);
#elif defined(_CC_MSVC_)
    return (int32_t)_InterlockedOr(a, v);
#elif defined(_CC_GNUC_)
    return __atomic_fetch_or(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_or(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int32_t) _cc_atomic32_xor(_cc_atomic32_t *a, int32_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_xor(a, v);
#elif defined(_CC_MSVC_)
    return (int32_t)_InterlockedXor(a, v);
#elif defined(_CC_GNUC_)
    return __atomic_fetch_xor(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_xor(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(bool_t) _cc_atomic32_cas(_cc_atomic32_t *a, int32_t v1, int32_t v2) {
#if __STDC_VERSION__ >= 201000L
    return (bool_t)atomic_compare_exchange_strong(a, &v1, v2);
#elif defined(_CC_MSVC_)
#if (defined(_M_IA64) || defined(_M_AMD64)) && !defined(RC_INVOKED)
    return (bool_t)((int32_t)InterlockedCompareExchange(a, v2, v1) == v1);
#else
    return (bool_t)((int32_t)InterlockedCompareExchange((long *)a, v2, v1) == v1);
#endif
#elif defined(_CC_GNUC_)
    return __atomic_compare_exchange_n(a, &v1, v2, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#else
    return (bool_t)__sync_bool_compare_and_swap(a, v1, v2);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int64_t) _cc_atomic64_add(_cc_atomic64_t *a, int64_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_add(a, v);
#elif defined(_CC_MSVC_)
    return (int64_t)InterlockedExchangeAdd64(a, v);
#elif defined(_CC_GNUC_)
    return __atomic_fetch_add(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_add(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int64_t) _cc_atomic64_sub(_cc_atomic64_t *a, int64_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_sub(a, v);
#elif defined(_CC_MSVC_)
    return (int64_t)InterlockedExchangeAdd64(a, -v);
#elif defined(_CC_GNUC_)
    return __atomic_fetch_sub(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_sub(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int64_t) _cc_atomic64_set(_cc_atomic64_t *a, int64_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_exchange(a, v);
#elif defined(_CC_MSVC_)
    return (int64_t)InterlockedExchange64(a, v);
#elif defined(_CC_GNUC_)
    return __atomic_exchange_n(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_lock_test_and_set(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int64_t) _cc_atomic64_and(_cc_atomic64_t *a, int64_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_and(a, v);
#elif defined(_CC_MSVC_)
    return (int64_t)InterlockedAnd64(a, v);
#elif defined(_CC_GNUC_)
    return __atomic_fetch_and(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_and_and_fetch(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int64_t) _cc_atomic64_or(_cc_atomic64_t *a, int64_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_or(a, v);
#elif defined(_CC_MSVC_)
    return (int64_t)InterlockedOr64(a, v);
#elif defined(_CC_GNUC_)
    return __atomic_fetch_or(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_or(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(int64_t) _cc_atomic64_xor(_cc_atomic64_t *a, int64_t v) {
#if __STDC_VERSION__ >= 201000L
    return atomic_fetch_xor(a, v);
#elif defined(_CC_MSVC_)
    return (int64_t)InterlockedXor64(a, v);
#elif defined(_CC_GNUC_)
    return __atomic_fetch_xor(a, v, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_xor(a, v);
#endif/*__STDC_VERSION__ >= 201000L*/
}

_CC_API_PUBLIC(bool_t) _cc_atomic64_cas(_cc_atomic64_t *a, int64_t v1, int64_t v2) {
#if __STDC_VERSION__ >= 201000L
    return (bool_t)atomic_compare_exchange_strong(a, &v1, v2);
#elif defined(_CC_MSVC_)
    return (bool_t)((int64_t)InterlockedCompareExchange64(a, v2, v1) == v1);
#elif defined(_CC_GNUC_)
    return __atomic_compare_exchange_n(a, &v1, v2, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#else
    return (bool_t)__sync_bool_compare_and_swap(a, v1, v2);
#endif/*__STDC_VERSION__ >= 201000L*/
}
