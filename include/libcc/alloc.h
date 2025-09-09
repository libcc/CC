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
#ifndef _C_CC_MEMORY_H_INCLUDED_
#define _C_CC_MEMORY_H_INCLUDED_

#include "logger.h"
#include "list.h"
#include "string.h"

#ifdef _CC_USE_CC_DEBUG_MALLOC_

#if defined(__CC_USE_TCMALLOC__)
    #include <google/tcmalloc.h>
    #if (TC_VERSION_MAJOR == 1 && TC_VERSION_MINOR >= 6) || (TC_VERSION_MAJOR > 1)
        #define _cc_malloc_size(p) tc_malloc_size(p)
    #else
        #error "Newer version of tcmalloc required"
    #endif
#elif defined(__CC_USE_JEMALLOC__)
    #include <jemalloc/jemalloc.h>
    #if (JEMALLOC_VERSION_MAJOR == 2 && JEMALLOC_VERSION_MINOR >= 1) || (JEMALLOC_VERSION_MAJOR > 2)
        #define _cc_malloc_size(p) je_malloc_usable_size(p)
    #else
        #error "Newer version of jemalloc required"
    #endif
#elif defined(__CC_APPLE__)
    #include <malloc/malloc.h>
    #define _cc_malloc_size(p) malloc_size(p)
#endif

#endif /* _CC_USE_CC_DEBUG_MALLOC_ */

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _CC_USE_CC_DEBUG_MALLOC_
/**/
_CC_API_PUBLIC(pvoid_t) _cc_debug_malloc(size_t n, const tchar_t *file, const int line);
/**/
_CC_API_PUBLIC(pvoid_t) _cc_debug_calloc(size_t c, size_t n, const tchar_t *file, const int line);
/**/
_CC_API_PUBLIC(pvoid_t) _cc_debug_realloc(pvoid_t d, size_t n, const tchar_t *file, const int line);
/**/
_CC_API_PUBLIC(void) _cc_debug_free(pvoid_t p);
/**/
_CC_API_PUBLIC(wchar_t*) _cc_debug_strdupW(const wchar_t *str, const tchar_t *file, const int line);
/**/
_CC_API_PUBLIC(char_t*) _cc_debug_strdupA(const char_t *str, const tchar_t *file, const int line);
/**/
_CC_API_PUBLIC(wchar_t*) _cc_debug_strndupW(const wchar_t *str, size_t n, const tchar_t *file, const int line);
/**/
_CC_API_PUBLIC(char_t*) _cc_debug_strndupA(const char_t *str, size_t n, const tchar_t *file, const int line);

/**/
#define _cc_malloc(N) _cc_debug_malloc((N), _CC_FILE_, _CC_LINE_)
/**/
#define _cc_calloc(C, N) _cc_debug_calloc((C), (N), _CC_FILE_, _CC_LINE_)
/**/
#define _cc_realloc(P, N) _cc_debug_realloc((P),(N), _CC_FILE_, _CC_LINE_)
/**/
#define _cc_free(P) _cc_debug_free(P)
/**/
#define _cc_strdupA(P) _cc_debug_strdupA((P), _CC_FILE_, _CC_LINE_)
/**/
#define _cc_strdupW(P) _cc_debug_strdupW((P), _CC_FILE_, _CC_LINE_)
/**/
#define _cc_strndupA(P,N) _cc_debug_strndupA((P), (N), _CC_FILE_, _CC_LINE_)
/**/
#define _cc_strndupW(P,N) _cc_debug_strndupW((P), (N), _CC_FILE_, _CC_LINE_)

#else

/**/
_CC_API_PUBLIC(pvoid_t) _cc_malloc(size_t);
/**/
_CC_API_PUBLIC(pvoid_t) _cc_calloc(size_t, size_t);
/**/
_CC_API_PUBLIC(pvoid_t) _cc_realloc(pvoid_t, size_t);
/**/
_CC_API_PUBLIC(void) _cc_free(pvoid_t);
/**/
_CC_API_PUBLIC(char_t*) _cc_strdupA(const char_t*);
/**/
_CC_API_PUBLIC(wchar_t*) _cc_strdupW(const wchar_t*);

/**/
_CC_API_PUBLIC(char_t*) _cc_strndupA(const char_t*,size_t);
/**/
_CC_API_PUBLIC(wchar_t*) _cc_strndupW(const wchar_t*,size_t);

#endif /* _CC_USE_CC_DEBUG_MALLOC_ */

/**/
#ifdef _CC_UNICODE_
    #define _cc_tcsdup(d)       _cc_strdupW(d)
    #define _cc_tcsndup(d,n)    _cc_strndupW(d,n)
#else
    #define _cc_tcsdup(d)       _cc_strdupA(d)
    #define _cc_tcsndup(d,n)     _cc_strndupA(d,n)
#endif

/**/
#define _cc_safe_free(d) do {\
    if ((d)) {\
        _cc_free((d));\
        (d) = nullptr;\
    }\
} while (0)

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif 

#endif/*_C_CC_MEMORY_H_INCLUDED_*/
