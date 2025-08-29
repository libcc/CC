#include <libcc/crypto/hmac.h>
#include <libcc/string.h>
#include <libcc/alloc.h>
#include <assert.h>

void test_hmac_alloc_free() {
    _cc_hmac_t *hmac = _cc_hmac_alloc(_CC_HMAC_SHA256_);
    assert(hmac != nullptr);
    _cc_hmac_free(hmac);
}

void test_hmac_init_update_final() {
    _cc_hmac_t *hmac = _cc_hmac_alloc(_CC_HMAC_SHA256_);
    assert(hmac != nullptr);

    const byte_t key[] = "test_key";
    const byte_t input[] = "test_input";
    byte_t output[64];

    _cc_hmac_init(hmac, key, sizeof(key) - 1);
    _cc_hmac_update(hmac, input, sizeof(input) - 1);
    int result = _cc_hmac_final(hmac, output, sizeof(output));
    assert(result > 0);

    _cc_hmac_free(hmac);
}

void test_hmac_invalid_type() {
    _cc_hmac_t *hmac = _cc_hmac_alloc(0xFF); // Invalid type
    assert(hmac == nullptr);
}

void test_hmac_key_too_long() {
    _cc_hmac_t *hmac = _cc_hmac_alloc(_CC_HMAC_SHA256_);
    assert(hmac != nullptr);

    byte_t long_key[128];
    memset(long_key, 'A', sizeof(long_key));

    _cc_hmac_init(hmac, long_key, sizeof(long_key));
    _cc_hmac_free(hmac);
}

int main() {
    test_hmac_alloc_free();
    test_hmac_init_update_final();
    test_hmac_invalid_type();
    test_hmac_key_too_long();
    return 0;
}