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
#ifndef _C_CC_WIDGETS_GZIP_H_INCLUDED_
#define _C_CC_WIDGETS_GZIP_H_INCLUDED_

#include "dylib.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define _GZIP_INF_ 1
#define _GZIP_DEF_ 2

typedef struct _gzip _gzip_t;

/**/
_CC_WIDGETS_API(_gzip_t*) _gzip_alloc(byte_t m);
/**/
_CC_WIDGETS_API(void) _gzip_free(_gzip_t *gzip);
/**/
_CC_WIDGETS_API(void) _gzip_reset(_gzip_t *gzip);
/**/
_CC_WIDGETS_API(bool_t) _gzip(_gzip_t *gzip, byte_t *source, size_t length, _cc_buf_t *buffer);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_HTTP_UTILS_H_INCLUDED_*/
