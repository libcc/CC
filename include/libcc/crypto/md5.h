#ifndef _C_CC_MD5_H_INCLUDED_
#define _C_CC_MD5_H_INCLUDED_

#include "hash.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define _CC_MD5_DIGEST_LENGTH_ 16

_CC_API_PUBLIC(void) _cc_md5_hash_init(_cc_hash_t *ctx);

/**
 * @brief          MD5 context structure
 */
typedef struct _cc_md5 {
    uint32_t total[2]; /*!< number of bytes processed  */
    uint32_t state[4]; /*!< intermediate digest state  */
    byte_t buffer[64]; /*!< data block being processed */
} _cc_md5_t;

/**
 * @brief          MD5 context setup
 *
 * @param ctx      context to be initialized
 */
_CC_API_PUBLIC(void) _cc_md5_init(_cc_md5_t* ctx);
/**
 * @brief          MD5 process buffer
 *
 * @param ctx      MD5 context
 * @param input    buffer holding the  data
 * @param ilen     length of the input data
 */
_CC_API_PUBLIC(void) _cc_md5_update(_cc_md5_t* ctx, const byte_t* input, size_t ilen);
/**
 * @brief          MD5 final digest
 *
 * @param ctx      MD5 context
 * @param output   MD5 checksum result
 */
_CC_API_PUBLIC(void) _cc_md5_final(_cc_md5_t* ctx, byte_t* output);
/**
 * @brief         Digests a file.
 *
 * @param fp       FILE handle
 * @param output   MD5 checksum result
 */
_CC_API_PUBLIC(bool_t) _cc_md5_fp(FILE* fp, tchar_t* output);
/**
 * @brief         Digests a file.
 *
 * @param fp       FILE handle
 * @param output   MD5 checksum result
 */
_CC_API_PUBLIC(bool_t) _cc_md5file(const tchar_t* filename, tchar_t* output);
/**
 * @brief          Output = MD5( input buffer )
 *
 * @param input    buffer holding the  data
 * @param ilen     length of the input data
 * @param output   MD5 checksum result
 */
_CC_API_PUBLIC(void) _cc_md5(const byte_t* input, size_t ilen, tchar_t* output);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_MD5_H_INCLUDED_*/
