#include <libcc/crypto/base58.h>
#include <libcc/alloc.h>
#include <assert.h>

void test_base58_encode() {
    const byte_t input[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    tchar_t output[100] = {0};
    size_t result = _cc_base58_encode(input, sizeof(input), output, sizeof(output));
    assert(result > 0);
    assert(output[0] != 0);
}

void test_base58_decode() {
    const tchar_t input[] = _T("123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz");
    byte_t output[100] = {0};
    size_t result = _cc_base58_decode(input, sizeof(input) - 1, output, sizeof(output));
    assert(result > 0);
    assert(output[0] != 0);
}

void test_base58_encode_empty_input() {
    tchar_t output[100] = {0};
    size_t result = _cc_base58_encode(NULL, 0, output, sizeof(output));
    assert(result == 0);
}

void test_base58_decode_invalid_input() {
    const tchar_t input[] = _T("!@#$%^&*()");
    byte_t output[100] = {0};
    size_t result = _cc_base58_decode(input, sizeof(input) - 1, output, sizeof(output));
    assert(result == 0);
}

int main() {
    test_base58_encode();
    test_base58_decode();
    test_base58_encode_empty_input();
    test_base58_decode_invalid_input();
    return 0;
}