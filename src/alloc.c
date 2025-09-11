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
#include <libcc/alloc.h>

#ifndef _CC_USE_DEBUG_MALLOC_

/* Explicitly override malloc/free etc when using tcmalloc. */
#if defined(__CC_USE_TCMALLOC__)
    #define malloc(size) tc_malloc(size)
    #define calloc(count, size) tc_calloc(count, size)
    #define realloc(ptr, size) tc_realloc(ptr, size)
    #define free(ptr) tc_free(ptr)
#elif defined(__CC_USE_JEMALLOC__)
    #define malloc(size) je_malloc(size)
    #define calloc(count, size) je_calloc(count, size)
    #define realloc(ptr, size) je_realloc(ptr, size)
    #define free(ptr) je_free(ptr)
#endif

/**/
_CC_API_PRIVATE(pvoid_t) _out_of_memory_abort(pvoid_t ptr, size_t size, const tchar_t *tp) {
    if (_cc_unlikely(nullptr == ptr)) {
        _cc_logger_error(_T("%s: Out of memory trying to allocate %zu bytes"), tp, size);
        _cc_abort();
    }
    return ptr;
}

/**/
_CC_API_PUBLIC(pvoid_t) _cc_malloc(size_t n) {
    return _out_of_memory_abort(malloc(n), n, _T("_cc_malloc"));
}

/**/
_CC_API_PUBLIC(pvoid_t) _cc_calloc(size_t c, size_t n) {
    return _out_of_memory_abort(calloc(c, n), c * n, _T("_cc_calloc"));
}

/**/
_CC_API_PUBLIC(pvoid_t) _cc_realloc(pvoid_t d, size_t n) {
    if (_cc_unlikely(n <= 0)) {
        _cc_free(d);
        return nullptr;
    }

    if (_cc_unlikely(d == nullptr)) {
        return _cc_malloc(n);
    }

    return _out_of_memory_abort(realloc(d, n), n, _T("_cc_realloc"));
}

/**/
_CC_API_PUBLIC(void) _cc_free(pvoid_t p) {
    _cc_assert(p != nullptr);
    free(p);
}

/**/
_CC_API_PUBLIC(wchar_t*) _cc_strdupW(const wchar_t *str) {
    return _cc_strndupW(str, wcslen(str));
}

/**/
_CC_API_PUBLIC(char_t*) _cc_strdupA(const char_t *str) {
    return _cc_strndupA(str, strlen(str));
}

/**/
_CC_API_PUBLIC(wchar_t*) _cc_strndupW(const wchar_t *str, size_t n) {
    wchar_t *p;

    if (_cc_unlikely(n <= 0)) {
        return nullptr;
    }

    p = (wchar_t *)_cc_malloc(sizeof(wchar_t) * (n + 1));
    if (_cc_likely(p)) {
        memcpy(p, str, n * sizeof(wchar_t));
        p[n] = 0;
    }

    return p;
}

/**/
_CC_API_PUBLIC(char_t*) _cc_strndupA(const char_t *str, size_t n) {
    char_t *p;

    if (_cc_unlikely(n <= 0)) {
        return nullptr;
    }

    p = (char_t *)_cc_malloc(sizeof(char_t) * (n + 1));
    if (_cc_likely(p)) {
        memcpy(p, str, n * sizeof(char_t));
        p[n] = 0;
    }

    return p;
}

#endif