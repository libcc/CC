#ifndef _C_CC_XXTEA_H_INCLUDED_
#define _C_CC_XXTEA_H_INCLUDED_

#include "../types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Function: _cc_xxtea_encrypt
 * @data:    Data to be encrypted
 * @len:     Length of the data to be encrypted
 * @key:     Symmetric key
 * @out_len: Pointer to output length variable
 * Returns:  Encrypted data or %nullptr on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
_CC_API_PUBLIC(byte_t*) _cc_xxtea_encrypt(const byte_t *data, size_t len, const byte_t *key, size_t *output_length);

/**
 * Function: xxtea_decrypt
 * @data:    Data to be decrypted
 * @len:     Length of the data to be decrypted
 * @key:     Symmetric key
 * @out_len: Pointer to output length variable
 * Returns:  Decrypted data or %nullptr on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
_CC_API_PUBLIC(byte_t*) _cc_xxtea_decrypt(const byte_t *data, size_t len, const byte_t *key, size_t *output_length);

#ifdef __cplusplus
}
#endif

#endif
