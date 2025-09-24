#include <libcc/crypto/xxtea.h>
#include <libcc/alloc.h>
#include <libcc/endian.h>
#include <assert.h>
#include <string.h>

void test_cc_xxtea_encrypt_decrypt() {
    const byte_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    const byte_t key[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00};
    size_t output_length;
    byte_t *encrypted = _cc_xxtea_encrypt(data, sizeof(data), key, &output_length);
    assert(encrypted != NULL);
    byte_t *decrypted = _cc_xxtea_decrypt(encrypted, output_length, key, &output_length);
    assert(decrypted != NULL);
    assert(memcmp(data, decrypted, sizeof(data)) == 0);
    _cc_free(encrypted);
    _cc_free(decrypted);
}

int main() {
    test_cc_xxtea_encrypt_decrypt();
    return 0;
}