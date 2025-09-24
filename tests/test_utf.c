#include <libcc/UTF.h>
#include <libcc/alloc.h>
#include <stdio.h>
#include <assert.h>

void test_convert_utf16_literal_to_utf8() {
    const tchar_t *input = "\\u0041\\u0042\\u4e2d\\u6587";
    const tchar_t *input_end = input + 16;
    tchar_t output[16];
    size_t output_length = sizeof(output);

    int32_t result = _cc_convert_utf16_literal_to_utf8(&input, input_end, output, output_length);
    assert(result == 6); // 6 bytes for "AB中文" in UTF-8
    assert(output[0] == 'A');
    assert(output[1] == 'B');
    assert(output[2] == 0xE4); // First byte of "中" in UTF-8
    assert(output[3] == 0xB8); // Second byte of "中" in UTF-8
    assert(output[4] == 0xAD); // Third byte of "中" in UTF-8
    assert(output[5] == 0xE6); // First byte of "文" in UTF-8
}

void test_unicode_to_utf16() {
    uint16_t utf16[2];
    size_t size = sizeof(utf16);

    // Test BMP character
    int32_t result = _cc_unicode_to_utf16(0x0041, utf16, size);
    assert(result == 1);
    assert(utf16[0] == 0x0041);

    // Test surrogate pair
    result = _cc_unicode_to_utf16(0x1F600, utf16, size);
    assert(result == 2);
    assert(utf16[0] >= 0xD800 && utf16[0] <= 0xDBFF); // High surrogate
    assert(utf16[1] >= 0xDC00 && utf16[1] <= 0xDFFF); // Low surrogate
}

void test_unicode_to_utf8() {
    uint8_t utf8[4];
    size_t size = sizeof(utf8);

    // Test 1-byte UTF-8
    int32_t result = _cc_unicode_to_utf8(0x0041, utf8, size);
    assert(result == 1);
    assert(utf8[0] == 0x41);

    // Test 2-byte UTF-8
    result = _cc_unicode_to_utf8(0x00A9, utf8, size);
    assert(result == 2);
    assert(utf8[0] == 0xC2);
    assert(utf8[1] == 0xA9);

    // Test 3-byte UTF-8
    result = _cc_unicode_to_utf8(0x4E2D, utf8, size);
    assert(result == 3);
    assert(utf8[0] == 0xE4);
    assert(utf8[1] == 0xB8);
    assert(utf8[2] == 0xAD);
}

void test_utf8_to_unicode() {
    uint32_t unic;

    // Test 1-byte UTF-8
    uint8_t utf8_1[] = {0x41, 0x00};
    int32_t result = _cc_utf8_to_unicode(utf8_1, sizeof(utf8_1), &unic);
    assert(result == 1);
    assert(unic == 0x0041);

    // Test 2-byte UTF-8
    uint8_t utf8_2[] = {0xC2, 0xA9, 0x00};
    result = _cc_utf8_to_unicode(utf8_2, sizeof(utf8_2), &unic);
    assert(result == 2);
    assert(unic == 0x00A9);

    // Test 3-byte UTF-8
    uint8_t utf8_3[] = {0xE4, 0xB8, 0xAD, 0x00};
    result = _cc_utf8_to_unicode(utf8_3, sizeof(utf8_3), &unic);
    assert(result == 3);
    assert(unic == 0x4E2D);
}

void test_utf32_to_utf16() {
    uint32_t utf32[] = {0x0041, 0x1F600, 0x0000};
    uint16_t utf16[4];
    uint16_t *utf16_end = utf16 + 4;

    int32_t result = _cc_utf32_to_utf16(utf32, utf32 + 2, utf16, utf16_end);
    assert(result == 3); // 1 for 'A', 2 for emoji
    assert(utf16[0] == 0x0041);
    assert(utf16[1] >= 0xD800 && utf16[1] <= 0xDBFF); // High surrogate
    assert(utf16[2] >= 0xDC00 && utf16[2] <= 0xDFFF); // Low surrogate
}

void test_utf16_to_utf8() {
    uint16_t utf16[] = {0x0041, 0x1F600, 0x0000};
    uint8_t utf8[6];
    uint8_t *utf8_end = utf8 + 6;

    int32_t result = _cc_utf16_to_utf8(utf16, utf16 + 2, utf8, utf8_end);
    assert(result == 5); // 1 for 'A', 4 for emoji
    assert(utf8[0] == 0x41);
    assert(utf8[1] == 0xF0); // First byte of emoji in UTF-8
    assert(utf8[2] == 0x9F); // Second byte of emoji in UTF-8
    assert(utf8[3] == 0x98); // Third byte of emoji in UTF-8
    assert(utf8[4] == 0x80); // Fourth byte of emoji in UTF-8
}

void test_utf8_to_utf16() {
    uint8_t utf8[] = {0x41, 0xF0, 0x9F, 0x98, 0x80, 0x00}; // 'A' and emoji
    uint16_t utf16[3];
    uint16_t *utf16_end = utf16 + 3;

    int32_t result = _cc_utf8_to_utf16(utf8, utf8 + 5, utf16, utf16_end);
    assert(result == 2); // 1 for 'A', 1 for emoji (surrogate pair)
    assert(utf16[0] == 0x0041);
    assert(utf16[1] >= 0xD800 && utf16[1] <= 0xDBFF); // High surrogate
    assert(utf16[2] >= 0xDC00 && utf16[2] <= 0xDFFF); // Low surrogate
}

int main() {
    test_convert_utf16_literal_to_utf8();
    test_unicode_to_utf16();
    test_unicode_to_utf8();
    test_utf8_to_unicode();
    test_utf32_to_utf16();
    test_utf16_to_utf8();
    test_utf8_to_utf16();
    printf("All UTF tests passed!\n");
    return 0;
}