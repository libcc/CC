#ifndef _C_CC_MD4_H_INCLUDED_
#define _C_CC_MD4_H_INCLUDED_

#include "../platform.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define _CC_MD4_DIGEST_LENGTH_ 16

/**
 * @brief          MD4 context structure
 */
typedef struct _cc_md4 {
    uint32_t total[2]; /*!< number of bytes processed  */
    uint32_t state[4]; /*!< intermediate digest state  */
    byte_t buffer[64]; /*!< data block being processed */
} _cc_md4_t;

/**
 * @brief          MD4 context setup
 *
 * @param ctx      context to be initialized
 */
_CC_API_PUBLIC(void) _cc_md4_init(_cc_md4_t* ctx);
/**
 * @brief          MD4 process buffer
 *
 * @param ctx      MD4 context
 * @param input    buffer holding the  data
 * @param ilen     length of the input data
 */
_CC_API_PUBLIC(void) _cc_md4_update(_cc_md4_t* ctx, const byte_t* input, size_t ilen);
/**
 * @brief          MD4 final digest
 *
 * @param ctx      MD4 context
 * @param output   MD4 checksum result
 */
_CC_API_PUBLIC(void) _cc_md4_final(_cc_md4_t* ctx, byte_t* output);

/**
 * @brief          Digests a file.
 *
 * @param fp       FILE handle
 * @param output   MD4 checksum result
 */
_CC_API_PUBLIC(bool_t) _cc_md4_fp(FILE* fp, tchar_t* output);

/**
 * @brief          Digests a file.
 *
 * @param filename       FILE handle
 * @param output   MD4 checksum result
 */
_CC_API_PUBLIC(bool_t) _cc_md4file(const tchar_t* filename, tchar_t* output);
/**
 * @brief          Output = MD4( input buffer )
 *
 * @param input    buffer holding the  data
 * @param length   length of the input data
 * @param output   MD4 checksum result
 */
_CC_API_PUBLIC(void) _cc_md4(const byte_t* input, size_t length, tchar_t* output);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_MD2_H_INCLUDED_*/
