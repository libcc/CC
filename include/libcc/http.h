#ifndef _C_CC_HTTP_C_H_INCLUDED_
#define _C_CC_HTTP_C_H_INCLUDED_

#include <math.h>
#include <wchar.h>
#include "sds.h"
#include "rbtree.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/*http methods*/
enum {
    _CC_HTTP_METHOD_INVALID_ = 0,
    _CC_HTTP_METHOD_GET_ = 1,
    _CC_HTTP_METHOD_POST_,
    _CC_HTTP_METHOD_PUT_,
    _CC_HTTP_METHOD_HEAD_,
    _CC_HTTP_METHOD_DELETE_,
    _CC_HTTP_METHOD_OPTIONS_,
    _CC_HTTP_METHOD_TRACE_,
    _CC_HTTP_METHOD_CONNECT_
};

enum {
    _CC_HTTP_STATUS_HEADER_ = 0,
    _CC_HTTP_STATUS_PAYLOAD_ = 1,
    _CC_HTTP_STATUS_ESTABLISHED_ = 2,
    _CC_HTTP_ERROR_UNIMPLEMENTED_,
    _CC_HTTP_ERROR_NOFOUND_,
    _CC_HTTP_ERROR_BADREQUEST_,
    _CC_HTTP_ERROR_TOOLARGE_,
};

/*
 * Define maximum number of headers that we accept.
 * This should be big enough to handle legitimate cases,
 * but limited to avoid DoS.
 */
#define _CC_HTTP_MAX_HEADERS_   256

typedef struct _cc_http_header {
    _cc_sds_t keyword;
    _cc_sds_t value;
    _cc_rbtree_iterator_t lnk;
} _cc_http_header_t;

typedef struct _cc_http_request_header {
    _cc_sds_t method;
    _cc_sds_t script;
    _cc_sds_t protocol;

    _cc_rbtree_t headers;
    
} _cc_http_request_header_t;

/**/
typedef struct _cc_http_response_header {
    bool_t keep_alive;
    byte_t content_encoding;
    byte_t transfer_encoding;
    
    int32_t status;

    uint64_t download_length;
    uint64_t content_length;

    _cc_sds_t protocol;
    _cc_sds_t description;
    _cc_sds_t location;

    _cc_rbtree_t headers;
} _cc_http_response_header_t;

typedef bool_t (*_cc_http_header_fn_t)(pvoid_t *arg, tchar_t *line, int32_t length);

_CC_API_PUBLIC(_cc_http_header_t*) _cc_http_header_alloc(void);
/**/
_CC_API_PUBLIC(void) _cc_http_header_free(_cc_http_header_t *m);
/**/
_CC_API_PUBLIC(bool_t) _cc_http_header_push(_cc_rbtree_t *ctx, _cc_http_header_t *data);
/**/
_CC_API_PUBLIC(const _cc_http_header_t*) _cc_http_header_find(_cc_rbtree_t *ctx, const tchar_t *keyword);
/**/
_CC_API_PUBLIC(void) _cc_http_header_destroy(_cc_rbtree_t *ctx);
/**/
_CC_API_PUBLIC(bool_t) _cc_http_header_line(_cc_rbtree_t *headers, tchar_t *line, int32_t length);
/**/
_CC_API_PUBLIC(int) _cc_http_header_parser(_cc_http_header_fn_t fn, pvoid_t *arg, byte_t *bytes, int32_t *length);
/**/
_CC_API_PUBLIC(bool_t) _cc_http_alloc_request_header(_cc_http_request_header_t **http_header, tchar_t *line, int32_t length);
/**/
_CC_API_PUBLIC(void) _cc_http_free_request_header(_cc_http_request_header_t **http_header);
/**/
_CC_API_PUBLIC(bool_t) _cc_http_alloc_response_header(_cc_http_response_header_t **url_response, tchar_t *line, int32_t length);
/**/
_CC_API_PUBLIC(void) _cc_http_free_response_header(_cc_http_response_header_t **response_header);
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_HTTP_C_H_INCLUDED_*/