#ifndef _C_CC_GZIP_H_INCLUDED_
#define _C_CC_GZIP_H_INCLUDED_

#include "buf.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define _GZIP_INF_ 1
#define _GZIP_DEF_ 2

typedef struct _gzip _gzip_t;

/**/
_CC_API_PUBLIC(_gzip_t*) _gzip_alloc(byte_t m);
/**/
_CC_API_PUBLIC(void) _gzip_free(_gzip_t *gzip);
/**/
_CC_API_PUBLIC(void) _gzip_reset(_gzip_t *gzip);
/**/
_CC_API_PUBLIC(bool_t) _gzip(_gzip_t *gzip, byte_t *source, size_t length, _cc_buf_t *buffer, bool_t finish);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_GZIP_H_INCLUDED_*/
