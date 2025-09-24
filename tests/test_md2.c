#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <libcc/crypto/md2.h>
#include <libcc/string.h>

void test_md2() {
    _cc_md2_t ctx;
    _cc_md2_init(&ctx);

    // 检查初始化后的状态
    for (int i = 0; i < 16; i++) {
        assert(ctx.cksum[i] == 0);
        assert(ctx.buffer[i] == 0);
    }
    for (int i = 0; i < 46; i++) {
        assert(ctx.state[i] == 0);
    }
    assert(ctx.left == 0);

    byte_t test_data[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                            0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    _cc_md2_update(&ctx, test_data, 16);

    byte_t output[16];
    _cc_md2_final(&ctx, output);

    // 检查输出摘要是否非空（具体值需根据算法逻辑验证）
    for (int i = 0; i < 16; i++) {
        assert(output[i] != 0);
    }

    printf("test_md2_final passed\n");
}

void test_md2_fp() {
    FILE *fp = fopen("test_file.bin", "wb");
    assert(fp != NULL);

    byte_t test_data[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                            0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    fwrite(test_data, sizeof(byte_t), 16, fp);
    fclose(fp);

    fp = fopen("test_file.bin", "rb");
    assert(fp != NULL);

    tchar_t output[_CC_MD2_DIGEST_LENGTH_ * 2 + 1];
    bool_t result = _cc_md2_fp(fp, output);
    assert(result == true);
    assert(strlen(output) == _CC_MD2_DIGEST_LENGTH_ * 2);

    fclose(fp);
    remove("test_file.bin");

    printf("test_md2_fp passed\n");
}

void test_md2file() {
    FILE *fp = fopen("test_file.bin", "wb");
    assert(fp != NULL);

    byte_t test_data[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                            0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    fwrite(test_data, sizeof(byte_t), 16, fp);
    fclose(fp);

    tchar_t output[_CC_MD2_DIGEST_LENGTH_ * 2 + 1];
    bool_t result = _cc_md2file(_T("test_file.bin"), output);
    assert(result == true);
    assert(strlen(output) == _CC_MD2_DIGEST_LENGTH_ * 2);

    remove("test_file.bin");

    printf("test_md2file passed\n");
}

int main() {
    test_md2();
    test_md2_fp();
    test_md2file();

    printf("All tests passed!\n");
    return 0;
}