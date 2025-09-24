#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <libcc/crypto/sha.h>
#include <libcc/string.h>

// Test SHA-3 initialization
void test_sha3_init() {
    _cc_sha3_t ctx;
    _cc_sha3_init(&ctx, _CC_SHA3_256_);

    assert(ctx.state[0][0] == 0);
    assert(ctx.state[4][4] == 0);
    assert(ctx.total == 0);
    assert(ctx.md_size == _CC_SHA3_MDSIZE(_CC_SHA3_256_));
    assert(ctx.block_size == _CC_SHA3_BLOCKSIZE(_CC_SHA3_256_));

    printf("test_sha3_init passed\n");
}

// Test SHA-3 process function
void test_sha3_process() {
    _cc_sha3_t ctx;
    _cc_sha3_init(&ctx, _CC_SHA3_256_);

    byte_t data[144] = {0};
    _cc_sha3_process(&ctx, data);

    // Verify state changes (exact values depend on SHA-3 algorithm)
    assert(ctx.state[0][0] != 0);
    assert(ctx.state[4][4] != 0);

    printf("test_sha3_process passed\n");
}

// Test SHA-3 update function
void test_sha3_update() {
    _cc_sha3_t ctx;
    _cc_sha3_init(&ctx, _CC_SHA3_256_);

    byte_t data[] = "abc";
    _cc_sha3_update(&ctx, data, strlen((const char*)data));

    // Verify total length
    assert(ctx.total == 3);

    printf("test_sha3_update passed\n");
}

// Test SHA-3 final function
void test_sha3_final() {
    _cc_sha3_t ctx;
    _cc_sha3_init(&ctx, _CC_SHA3_256_);

    byte_t data[] = "abc";
    _cc_sha3_update(&ctx, data, strlen((const char*)data));

    byte_t output[_CC_SHA3_256_DIGEST_LENGTH_];
    _cc_sha3_final(&ctx, output);

    // Expected SHA-3-256 hash for "abc"
    byte_t expected[] = {
        0x3A, 0x98, 0x5D, 0xA7, 0x4F, 0xE2, 0x25, 0xB2,
        0x04, 0x5C, 0x17, 0x2D, 0x6B, 0xD3, 0x90, 0xBD,
        0x85, 0x5F, 0x08, 0x6E, 0x3E, 0x9D, 0x52, 0x5B,
        0x46, 0xBF, 0xE2, 0x45, 0x11, 0x43, 0x15, 0x49
    };

    assert(memcmp(output, expected, _CC_SHA3_256_DIGEST_LENGTH_) == 0);

    printf("test_sha3_final passed\n");
}

// Test SHA-3 file pointer function
void test_sha3_fp() {
    FILE *fp = tmpfile();
    if (!fp) {
        perror("Failed to create temporary file");
        return;
    }

    fputs("abc", fp);
    rewind(fp);

    tchar_t output[_CC_SHA3_256_DIGEST_LENGTH_ * 2 + 1];
    assert(_cc_sha3_fp(fp, _CC_SHA3_256_, output) == true);

    // Expected SHA-3-256 hash for "abc"
    tchar_t expected[] = _T("3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431549");
    assert(_tcscmp(output, expected) == 0);

    fclose(fp);
    printf("test_sha3_fp passed\n");
}

// Test SHA-3 file function
void test_sha3file() {
    const tchar_t *filename = _T("testfile.txt");
    FILE *fp = _tfopen(filename, _T("w"));
    if (!fp) {
        perror("Failed to create test file");
        return;
    }

    fputs("abc", fp);
    fclose(fp);

    tchar_t output[_CC_SHA3_256_DIGEST_LENGTH_ * 2 + 1];
    assert(_cc_sha3file(filename, _CC_SHA3_256_, output) == true);

    // Expected SHA-3-256 hash for "abc"
    tchar_t expected[] = _T("3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431549");
    assert(_tcscmp(output, expected) == 0);

    remove("testfile.txt");
    printf("test_sha3file passed\n");
}

// Test SHA-3 direct input function
void test_sha3() {
    byte_t data[] = "abc";
    tchar_t output[_CC_SHA3_256_DIGEST_LENGTH_ * 2 + 1];
    _cc_sha3(data, strlen((const char*)data), _CC_SHA3_256_, output);

    // Expected SHA-3-256 hash for "abc"
    tchar_t expected[] = _T("3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431549");
    assert(_tcscmp(output, expected) == 0);

    printf("test_sha3 passed\n");
}

// Main function to run all tests
int main() {
    test_sha3_init();
    test_sha3_process();
    test_sha3_update();
    test_sha3_final();
    test_sha3_fp();
    test_sha3file();
    test_sha3();

    printf("All tests passed!\n");
    return 0;
}