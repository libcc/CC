#include "libcc/crypto/aes.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>

void _cc_aes_init(_cc_aes_t *ctx) {
    memset(ctx, 0, sizeof(_cc_aes_t));
}

int _cc_aes_setkey_enc(_cc_aes_t *ctx, const byte_t *key, unsigned int keybits) {
    if (keybits != 128 && keybits != 192 && keybits != 256)
        return _CC_ERR_AES_INVALID_KEY_LENGTH_;

    ctx->nr = keybits / 32 + 6; // OpenSSL rounds: 10 for 128, 12 for 192, 14 for 256
    return 0;
}

int _cc_aes_setkey_dec(_cc_aes_t *ctx, const byte_t *key, unsigned int keybits) {
    return _cc_aes_setkey_enc(ctx, key, keybits);
}

int _cc_aes_setkey(_cc_aes_t *ctx, int mode, const byte_t *key, uint32_t keybits) {
    return _cc_aes_setkey_enc(ctx, key, keybits);
}

#if defined(_CC_CIPHER_MODE_CBC_)
int _cc_aes_crypt_cbc(_cc_aes_t *ctx, int mode, size_t length, byte_t iv[16], const byte_t *input, byte_t *output) {
    EVP_CIPHER_CTX *evp_ctx;
    int len;
    int ret = 0;

    if (length % 16 != 0)
        return _CC_ERR_AES_INVALID_INPUT_LENGTH_;

    evp_ctx = EVP_CIPHER_CTX_new();
    if (!evp_ctx)
        return -1;

    const EVP_CIPHER *cipher;
    switch (ctx->nr) {
        case 10: cipher = EVP_aes_128_cbc(); break;
        case 12: cipher = EVP_aes_192_cbc(); break;
        case 14: cipher = EVP_aes_256_cbc(); break;
        default: return _CC_ERR_AES_INVALID_KEY_LENGTH_;
    }

    if (EVP_CipherInit_ex(evp_ctx, cipher, NULL, NULL, NULL, mode == _CC_AES_ENCRYPT_) != 1)
        goto cleanup;

    if (EVP_CipherInit_ex(evp_ctx, NULL, NULL, (const unsigned char *)ctx->rk, iv, mode == _CC_AES_ENCRYPT_) != 1)
        goto cleanup;

    if (EVP_CipherUpdate(evp_ctx, output, &len, input, length) != 1)
        goto cleanup;

    if (EVP_CipherFinal_ex(evp_ctx, output + len, &len) != 1)
        goto cleanup;

    ret = 0;

cleanup:
    EVP_CIPHER_CTX_free(evp_ctx);
    return ret;
}
#endif

#if defined(_CC_CIPHER_MODE_CFB_)
int _cc_aes_crypt_cfb128(_cc_aes_t *ctx, int mode, const byte_t *input, size_t length, size_t *iv_off, byte_t iv[16], byte_t *output) {
    EVP_CIPHER_CTX *evp_ctx;
    int len;
    int ret = 0;

    evp_ctx = EVP_CIPHER_CTX_new();
    if (!evp_ctx)
        return -1;

    const EVP_CIPHER *cipher;
    switch (ctx->nr) {
        case 10: cipher = EVP_aes_128_cfb128(); break;
        case 12: cipher = EVP_aes_192_cfb128(); break;
        case 14: cipher = EVP_aes_256_cfb128(); break;
        default: return _CC_ERR_AES_INVALID_KEY_LENGTH_;
    }

    if (EVP_CipherInit_ex(evp_ctx, cipher, NULL, (const unsigned char *)ctx->rk, iv, mode == _CC_AES_ENCRYPT_) != 1)
        goto cleanup;

    if (EVP_CipherUpdate(evp_ctx, output, &len, input, length) != 1)
        goto cleanup;

    ret = 0;

cleanup:
    EVP_CIPHER_CTX_free(evp_ctx);
    return ret;
}

int _cc_aes_crypt_cfb8(_cc_aes_t *ctx, int mode, const byte_t *input, size_t length, byte_t iv[16], byte_t *output) {
    EVP_CIPHER_CTX *evp_ctx;
    int len;
    int ret = 0;

    evp_ctx = EVP_CIPHER_CTX_new();
    if (!evp_ctx)
        return -1;

    const EVP_CIPHER *cipher;
    switch (ctx->nr) {
        case 10: cipher = EVP_aes_128_cfb8(); break;
        case 12: cipher = EVP_aes_192_cfb8(); break;
        case 14: cipher = EVP_aes_256_cfb8(); break;
        default: return _CC_ERR_AES_INVALID_KEY_LENGTH_;
    }

    if (EVP_CipherInit_ex(evp_ctx, cipher, NULL, (const unsigned char *)ctx->rk, iv, mode == _CC_AES_ENCRYPT_) != 1)
        goto cleanup;

    if (EVP_CipherUpdate(evp_ctx, output, &len, input, length) != 1)
        goto cleanup;

    ret = 0;

cleanup:
    EVP_CIPHER_CTX_free(evp_ctx);
    return ret;
}
#endif

#if defined(_CC_CIPHER_MODE_CTR_)
int _cc_aes_crypt_ctr(_cc_aes_t *ctx, const byte_t *input, size_t length, size_t *nc_off, byte_t nonce_counter[16], byte_t stream_block[16], byte_t *output) {
    EVP_CIPHER_CTX *evp_ctx;
    int len;
    int ret = 0;

    evp_ctx = EVP_CIPHER_CTX_new();
    if (!evp_ctx)
        return -1;

    const EVP_CIPHER *cipher;
    switch (ctx->nr) {
        case 10: cipher = EVP_aes_128_ctr(); break;
        case 12: cipher = EVP_aes_192_ctr(); break;
        case 14: cipher = EVP_aes_256_ctr(); break;
        default: return _CC_ERR_AES_INVALID_KEY_LENGTH_;
    }

    if (EVP_CipherInit_ex(evp_ctx, cipher, NULL, (const unsigned char *)ctx->rk, nonce_counter, 1) != 1)
        goto cleanup;

    if (EVP_CipherUpdate(evp_ctx, output, &len, input, length) != 1)
        goto cleanup;

    ret = 0;

cleanup:
    EVP_CIPHER_CTX_free(evp_ctx);
    return ret;
}
#endif