#include <libcc/crc.h>
#include <libcc/string.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

void test_crc8_lsb_table() {
    uint8_t *table = _cc_build_crc8_lsb_table(0xAB);
    assert(table != NULL);
    assert(table[0] == 0x00);
    assert(table[1] == 0xF7);
    assert(table[255] == 0x9F);
    printf("test_crc8_lsb_table passed\n");
}

void test_crc8_msb_table() {
    uint8_t *table = _cc_build_crc8_msb_table(0xAB);
    assert(table != NULL);
    assert(table[0] == 0x00);
    assert(table[1] == 0xAB);
    assert(table[255] == 0xAA);
    printf("test_crc8_msb_table passed\n");
}

void test_crc8() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t crc = _cc_crc8(data, sizeof(data), false);
    assert(crc == 0x6B);
    crc = _cc_crc8(data, sizeof(data), true);
    assert(crc == 0x9A);
    printf("test_crc8 passed\n");
}

void test_crc16() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint16_t crc = _cc_crc16(data, sizeof(data));
    assert(crc == 0x9D0F);
    printf("test_crc16 passed\n");
}

void test_crc32() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint32_t crc = _cc_crc32(data, sizeof(data));
    assert(crc == 0x7DF4E5E8);
    printf("test_crc32 passed\n");
}

void test_crc32_mpeg2() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint32_t crc = _cc_crc32_mpeg2(data, sizeof(data));
    assert(crc == 0x04C11DB7);
    printf("test_crc32_mpeg2 passed\n");
}

void test_crc64() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint64_t crc = _cc_crc64(data, sizeof(data));
    assert(crc == 0x7AD870C830358979);
    printf("test_crc64 passed\n");
}

void test_hash() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint32_t hash = _cc_hash(data, sizeof(data));
    assert(hash == 0x1D8A);
    printf("test_hash passed\n");
}

void test_hash_rotating() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint32_t hash = _cc_hash_rotating(data, sizeof(data));
    assert(hash == 0x05040302);
    printf("test_hash_rotating passed\n");
}

void test_hash_fnv1_32() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint32_t hash = _cc_hash_fnv1_32(data, sizeof(data));
    assert(hash == 0x811C9DC5);
    printf("test_hash_fnv1_32 passed\n");
}

void test_hash_fnv1a_32() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint32_t hash = _cc_hash_fnv1a_32(data, sizeof(data));
    assert(hash == 0x811C9DC5);
    printf("test_hash_fnv1a_32 passed\n");
}

void test_hash_fnv1_64() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint64_t hash = _cc_hash_fnv1_64(data, sizeof(data));
    assert(hash == 0xCBF29CE484222325);
    printf("test_hash_fnv1_64 passed\n");
}

void test_hash_fnv1a_64() {
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint64_t hash = _cc_hash_fnv1a_64(data, sizeof(data));
    assert(hash == 0xCBF29CE484222325);
    printf("test_hash_fnv1a_64 passed\n");
}

int main() {
    test_crc8_lsb_table();
    test_crc8_msb_table();
    test_crc8();
    test_crc16();
    test_crc32();
    test_crc32_mpeg2();
    test_crc64();
    test_hash();
    test_hash_rotating();
    test_hash_fnv1_32();
    test_hash_fnv1a_32();
    test_hash_fnv1_64();
    test_hash_fnv1a_64();
    printf("All tests passed!\n");
    return 0;
}