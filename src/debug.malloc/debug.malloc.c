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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "debug.tracked.c.h"

/**/
_CC_API_PRIVATE(pvoid_t) _out_of_memory_abort(pvoid_t ptr, size_t size, byte_t m_type, const tchar_t *file, const int line) {
    if (_cc_unlikely(nullptr == ptr)) {
        printf(_T("%s(%d):Out of memory trying to allocate %zu bytes"), file, line, size);
        abort();
    }
    return _debug_alloc_link(ptr, size, file, line, m_type);
}

/**/
_CC_API_PUBLIC(pvoid_t) _cc_debug_malloc(size_t n, const tchar_t *file, const int line) {
    return _out_of_memory_abort(malloc(n + sizeof(_cc_debug_alloc_t)), n, _CC_DEBUG_MALLOC_, file, line);
}

/**/
_CC_API_PUBLIC(pvoid_t) _cc_debug_calloc(size_t c, size_t n, const tchar_t *file, const int line) {
    pvoid_t ptr;
    n = c * n;
    ptr = _out_of_memory_abort(malloc(n + sizeof(_cc_debug_alloc_t)), n, _CC_DEBUG_CALLOC_, file, line);
    bzero(ptr, n);
    return ptr;
}

/**/
_CC_API_PUBLIC(pvoid_t) _cc_debug_realloc(pvoid_t d, size_t n, const tchar_t *file, const int line) {
    if (_cc_unlikely(n <= 0)) {
        _cc_debug_free(d);
        return nullptr;
    }

    if (_cc_unlikely(d == nullptr)) {
        return _cc_debug_malloc(n, file, line);
    }
    return _out_of_memory_abort(realloc(_debug_alloc_unlink(d), n + sizeof(_cc_debug_alloc_t)), n, _CC_DEBUG_REALLOC_, file, line);
}

/**/
_CC_API_PUBLIC(void) _cc_debug_free(pvoid_t p) {
    _cc_assert(p != nullptr);
    free(_debug_alloc_unlink(p));
}