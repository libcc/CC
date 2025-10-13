#ifndef _C_CC_SHA_H_INCLUDED_
#define _C_CC_SHA_H_INCLUDED_

#include "hash.h"

#define _CC_SHA1_DIGEST_LENGTH_       20
#define _CC_SHA224_DIGEST_LENGTH_     28
#define _CC_SHA256_DIGEST_LENGTH_     32
#define _CC_SHA384_DIGEST_LENGTH_     48
#define _CC_SHA512_DIGEST_LENGTH_     64

#define _CC_KECCAK1600_WIDTH_         1600

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

_CC_API_PUBLIC(void) _cc_sha1_init(_cc_hash_t *sha);
_CC_API_PUBLIC(void) _cc_sha224_init(_cc_hash_t *sha);
_CC_API_PUBLIC(void) _cc_sha256_init(_cc_hash_t *sha);
_CC_API_PUBLIC(void) _cc_sha384_init(_cc_hash_t *sha);
_CC_API_PUBLIC(void) _cc_sha512_init(_cc_hash_t *sha);

/**
 * @brief          Output = SHA-1( input buffer )
 *
 * @param input    buffer holding the  data
 * @param length     length of the input data
 * @param output   SHA-1
 */
_CC_API_PUBLIC(void)
_cc_sha1(const byte_t* input, size_t length, tchar_t* output);
/**
 * @brief         Digests a file.
 *
 * @param fp       FILE handle
 * @param output   SHA1 checksum result
 */
_CC_API_PUBLIC(bool_t)
_cc_sha1_fp(FILE* fp, tchar_t* output);
/*
    Digests a file.
 */
_CC_API_PUBLIC(bool_t)
_cc_sha1file(const tchar_t* filename, tchar_t* output);

/**
 * @brief          Output = SHA-256( input buffer )
 *
 * @param input    buffer holding the  data
 * @param length   length of the input data
 * @param output   SHA-256
 * @param is224    0 = use SHA256, 1 = use SHA224
 */
_CC_API_PUBLIC(void)
_cc_sha256(const byte_t* input, size_t length, tchar_t* output, bool_t is224);
/**
 * @brief         Digests a file.
 *
 * @param fp       FILE handle
 * @param output   SHA256 checksum result
 */
_CC_API_PUBLIC(bool_t)
_cc_sha256_fp(FILE *fp, tchar_t *output, bool_t is224);
/**
 * @brief          Output = SHA-256( input file path )
 *
 * @param filename    buffer holding the  data
 * @param output   SHA-256
 * @param is384    0 = use SHA256, 1 = use SHA224
 */
_CC_API_PUBLIC(bool_t)
_cc_sha256file(const tchar_t* filename, tchar_t* output, bool_t is224);

/**
 * @brief       Output = SHA-512( input buffer )
 *
 * @param input      buffer holding the  data
 * @param ilen        length of the input data
 * @param output    SHA-512
 * @param is384    0 = use SHA512, 1 = use SHA384
 */
_CC_API_PUBLIC(void)
_cc_sha512(const byte_t* input, size_t length, tchar_t* output, bool_t is384);
/**
 * @brief         Digests a file.
 *
 * @param fp       FILE handle
 * @param output   SHA-512 checksum result
 */
_CC_API_PUBLIC(bool_t)
_cc_sha512_fp(FILE *fp, tchar_t *output, bool_t is384);
/**
 * @brief       Output = SHA-512(  input file path )

 * @param filename    buffer holding the  data
 * @param output    SHA-512
 * @param is384    0 = use SHA512, 1 = use SHA384
 */
_CC_API_PUBLIC(bool_t)
_cc_sha512file(const tchar_t* filename, tchar_t* output, bool_t is384);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_SHA_H_INCLUDED_*/
