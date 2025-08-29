#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "libcc/crypto/des.h"

#define TEST_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            printf("Test failed at %s:%d\n", __FILE__, __LINE__); \
            return -1; \
        } \
    } while (0)

int test_des_init() {
    _cc_des_t ctx;
    _cc_des_init(&ctx);
    for (int i = 0; i < 32; i++) {
        TEST_ASSERT(ctx.sk[i] == 0);
    }
    return 0;
}

int test_des_key_set_parity() {
    byte_t key[_CC_DES_KEY_SIZE_] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    _cc_des_key_set_parity(key);
    TEST_ASSERT(_cc_des_key_check_key_parity(key));
    return 0;
}

int test_des_key_check_weak() {
    byte_t weak_key[_CC_DES_KEY_SIZE_] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    TEST_ASSERT(_cc_des_key_check_weak(weak_key));

    byte_t non_weak_key[_CC_DES_KEY_SIZE_] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    TEST_ASSERT(!_cc_des_key_check_weak(non_weak_key));
    return 0;
}

int test_des_setkey_enc_dec() {
    _cc_des_t ctx;
    byte_t key[_CC_DES_KEY_SIZE_] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    _cc_des_setkey_enc(&ctx, key);
    _cc_des_setkey_dec(&ctx, key);
    TEST_ASSERT(ctx.sk[0] != 0);
    return 0;
}

int test_des_crypt_ecb() {
    _cc_des_t ctx;
    byte_t key[_CC_DES_KEY_SIZE_] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    byte_t input[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    byte_t output[8];
    _cc_des_setkey_enc(&ctx, key);
    _cc_des_crypt_ecb(&ctx, input, output);
    TEST_ASSERT(memcmp(output, input, 8) != 0);
    return 0;
}

int test_des3_set2key_enc_dec() {
    _cc_des3_t ctx;
    byte_t key[_CC_DES_KEY_SIZE_ * 2] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                                        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    _cc_des3_set2key_enc(&ctx, key);
    _cc_des3_set2key_dec(&ctx, key);
    TEST_ASSERT(ctx.sk[0] != 0);
    return 0;
}

int test_des3_crypt_ecb() {
    _cc_des3_t ctx;
    byte_t key[_CC_DES_KEY_SIZE_ * 3] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                                        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                                        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    byte_t input[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    byte_t output[8];
    _cc_des3_set3key_enc(&ctx, key);
    _cc_des3_crypt_ecb(&ctx, input, output);
    TEST_ASSERT(memcmp(output, input, 8) != 0);
    return 0;
}

int main() {
    printf("Running DES tests...\n");
    if (test_des_init() != 0) return -1;
    if (test_des_key_set_parity() != 0) return -1;
    if (test_des_key_check_weak() != 0) return -1;
    if (test_des_setkey_enc_dec() != 0) return -1;
    if (test_des_crypt_ecb() != 0) return -1;
    if (test_des3_set2key_enc_dec() != 0) return -1;
    if (test_des3_crypt_ecb() != 0) return -1;
    printf("All tests passed!\n");
    return 0;
}