#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <libcc/crypto/sha.h>
#include <libcc/string.h>

// Test SHA-1 initialization
void test_sha1_init() {
    _cc_sha1_t ctx;
    _cc_sha1_init(&ctx);

    assert(ctx.state[0] == 0x67452301);
    assert(ctx.state[1] == 0xEFCDAB89);
    assert(ctx.state[2] == 0x98BADCFE);
    assert(ctx.state[3] == 0x10325476);
    assert(ctx.state[4] == 0xC3D2E1F0);
    assert(ctx.total[0] == 0);
    assert(ctx.total[1] == 0);

    printf("test_sha1_init passed\n");
}

// Test SHA-1 process function
void test_sha1_process() {
    _cc_sha1_t ctx;
    _cc_sha1_init(&ctx);

    byte_t data[64] = {
        0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
        0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
        0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
        0x57, 0x58, 0x59, 0x5A, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40, 0x23
    };

    _cc_sha1_process(&ctx, data);

    // Verify partial state changes (exact values depend on SHA-1 algorithm)
    assert(ctx.state[0] != 0x67452301);
    assert(ctx.state[1] != 0xEFCDAB89);
    assert(ctx.state[2] != 0x98BADCFE);
    assert(ctx.state[3] != 0x10325476);
    assert(ctx.state[4] != 0xC3D2E1F0);

    printf("test_sha1_process passed\n");
}

// Test SHA-1 update function
void test_sha1_update() {
    _cc_sha1_t ctx;
    _cc_sha1_init(&ctx);

    byte_t data[] = "abc";
    _cc_sha1_update(&ctx, data, strlen((const char*)data));

    // Verify total length
    assert(ctx.total[0] == 3);
    assert(ctx.total[1] == 0);

    printf("test_sha1_update passed\n");
}

// Test SHA-1 final function
void test_sha1_final() {
    _cc_sha1_t ctx;
    _cc_sha1_init(&ctx);

    byte_t data[] = "abc";
    _cc_sha1_update(&ctx, data, strlen((const char*)data));

    byte_t output[_CC_SHA1_DIGEST_LENGTH_];
    _cc_sha1_final(&ctx, output);

    // Expected SHA-1 hash for "abc"
    byte_t expected[] = {
        0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E,
        0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D
    };

    assert(memcmp(output, expected, _CC_SHA1_DIGEST_LENGTH_) == 0);

    printf("test_sha1_final passed\n");
}

// Test SHA-1 file pointer function
void test_sha1_fp() {
    FILE *fp = tmpfile();
    if (!fp) {
        perror("Failed to create temporary file");
        return;
    }

    fputs("abc", fp);
    rewind(fp);

    tchar_t output[_CC_SHA1_DIGEST_LENGTH_ * 2 + 1];
    assert(_cc_sha1_fp(fp, output) == true);

    // Expected SHA-1 hash for "abc"
    tchar_t expected[] = _T("a9993e364706816aba3e25717850c26c9cd0d89d");
    assert(_tcscmp(output, expected) == 0);

    fclose(fp);
    printf("test_sha1_fp passed\n");
}

// Test SHA-1 file function
void test_sha1file() {
    const tchar_t *filename = _T("testfile.txt");
    FILE *fp = _tfopen(filename, _T("w"));
    if (!fp) {
        perror("Failed to create test file");
        return;
    }

    fputs("abc", fp);
    fclose(fp);

    tchar_t output[_CC_SHA1_DIGEST_LENGTH_ * 2 + 1];
    assert(_cc_sha1file(filename, output) == true);

    // Expected SHA-1 hash for "abc"
    tchar_t expected[] = _T("a9993e364706816aba3e25717850c26c9cd0d89d");
    assert(_tcscmp(output, expected) == 0);

    remove("testfile.txt");
    printf("test_sha1file passed\n");
}

// Test SHA-1 direct input function
void test_sha1() {
    byte_t data[] = "abc";
    tchar_t output[_CC_SHA1_DIGEST_LENGTH_ * 2 + 1];
    _cc_sha1(data, strlen((const char*)data), output);

    // Expected SHA-1 hash for "abc"
    tchar_t expected[] = _T("a9993e364706816aba3e25717850c26c9cd0d89d");
    assert(_tcscmp(output, expected) == 0);

    printf("test_sha1 passed\n");
}

// Main function to run all tests
int main() {
    test_sha1_init();
    test_sha1_process();
    test_sha1_update();
    test_sha1_final();
    test_sha1_fp();
    test_sha1file();
    test_sha1();

    printf("All tests passed!\n");
    return 0;
}