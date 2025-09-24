#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libcc/crypto/sha.h>

/* Test cases for SHA-256 functionality */

/**
 * Test initialization of SHA-256 context.
 */
void test_sha256_init() {
    _cc_sha256_t ctx;
    _cc_sha256_init(&ctx, false);
    assert(ctx.state[0] == 0x6A09E667);
    assert(ctx.state[1] == 0xBB67AE85);
    assert(ctx.state[2] == 0x3C6EF372);
    assert(ctx.state[3] == 0xA54FF53A);
    assert(ctx.state[4] == 0x510E527F);
    assert(ctx.state[5] == 0x9B05688C);
    assert(ctx.state[6] == 0x1F83D9AB);
    assert(ctx.state[7] == 0x5BE0CD19);
    assert(ctx.is224 == false);

    _cc_sha256_init(&ctx, true);
    assert(ctx.state[0] == 0xC1059ED8);
    assert(ctx.state[1] == 0x367CD507);
    assert(ctx.state[2] == 0x3070DD17);
    assert(ctx.state[3] == 0xF70E5939);
    assert(ctx.state[4] == 0xFFC00B31);
    assert(ctx.state[5] == 0x68581511);
    assert(ctx.state[6] == 0x64F98FA7);
    assert(ctx.state[7] == 0xBEFA4FA4);
    assert(ctx.is224 == true);

    printf("test_sha256_init passed\n");
}

/**
 * Test SHA-256 process and update functions.
 */
void test_sha256_process_update() {
    _cc_sha256_t ctx;
    byte_t input[] = "abc";
    byte_t output[32];

    _cc_sha256_init(&ctx, false);
    _cc_sha256_update(&ctx, input, strlen((const char *)input));
    _cc_sha256_final(&ctx, output);

    // Expected SHA-256 hash of "abc"
    byte_t expected[] = {
        0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA,
        0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
        0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C,
        0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD
    };

    assert(memcmp(output, expected, 32) == 0);
    printf("test_sha256_process_update passed\n");
}

/**
 * Test SHA-256 file hashing.
 */
void test_sha256_file() {
    tchar_t output[65];
    const tchar_t *filename = _T("testfile.txt");
    FILE *fp = _tfopen(filename, _T("wb"));
    if (fp) {
        fwrite("abc", 1, 3, fp);
        fclose(fp);
    }

    assert(_cc_sha256file(filename, output, false) == true);
    assert(_tcscmp(output, _T("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad")) == 0);
    printf("test_sha256_file passed\n");

    remove("testfile.txt");
}

/**
 * Main function to run the tests.
 */
int main() {
    test_sha256_init();
    test_sha256_process_update();
    test_sha256_file();

    printf("All tests passed!\n");
    return 0;
}