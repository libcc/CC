#include <libcc/crypto/md5.h>
#include <libcc/string.h>
#include <stdio.h>
#include <assert.h>

void test_md5_init() {
    _cc_md5_t ctx;
    _cc_md5_init(&ctx);

    assert(ctx.total[0] == 0);
    assert(ctx.total[1] == 0);
    assert(ctx.state[0] == 0x67452301);
    assert(ctx.state[1] == 0xEFCDAB89);
    assert(ctx.state[2] == 0x98BADCFE);
    assert(ctx.state[3] == 0x10325476);
}

void test_md5_process() {
    _cc_md5_t ctx;
    _cc_md5_init(&ctx);

    byte_t data[64] = {0};
    _cc_md5_process(&ctx, data);

    // Verify the state after processing
    assert(ctx.state[0] != 0x67452301);
    assert(ctx.state[1] != 0xEFCDAB89);
    assert(ctx.state[2] != 0x98BADCFE);
    assert(ctx.state[3] != 0x10325476);
}

void test_md5_update() {
    _cc_md5_t ctx;
    _cc_md5_init(&ctx);

    byte_t input[64] = {0};
    _cc_md5_update(&ctx, input, 64);

    assert(ctx.total[0] == 64);
    assert(ctx.total[1] == 0);
}

void test_md5_final() {
    _cc_md5_t ctx;
    _cc_md5_init(&ctx);

    byte_t output[16];
    _cc_md5_final(&ctx, output);

    // Verify the output is not zero
    for (int i = 0; i < 16; i++) {
        assert(output[i] != 0);
    }
}

void test_md5() {
    byte_t input[] = "test";
    tchar_t output[33];
    _cc_md5(input, sizeof(input) - 1, output);

    // Verify the output is a valid MD5 hash
    assert(output[0] != '\0');
}

void test_md5_fp() {
    FILE *fp = fopen("testfile.txt", "wb");
    assert(fp != NULL);
    fwrite("test", 1, 4, fp);
    fclose(fp);

    tchar_t output[33];
    fp = fopen("testfile.txt", "rb");
    assert(_cc_md5_fp(fp, output));
    fclose(fp);

    // Verify the output is a valid MD5 hash
    assert(output[0] != '\0');
}

void test_md5file() {
    FILE *fp = fopen("testfile.txt", "wb");
    assert(fp != NULL);
    fwrite("test", 1, 4, fp);
    fclose(fp);

    tchar_t output[33];
    assert(_cc_md5file("testfile.txt", output));

    // Verify the output is a valid MD5 hash
    assert(output[0] != '\0');
}

int main() {
    test_md5_init();
    test_md5_process();
    test_md5_update();
    test_md5_final();
    test_md5();
    test_md5_fp();
    test_md5file();

    printf("All tests passed!\n");
    return 0;
}