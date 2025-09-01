# xxtea.h 文档

## 文件概述
`xxtea.h` 是一个实现 XXTEA 加密算法的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. XXTEA 加密/解密
提供了以下函数：
- `_cc_xxtea_encrypt` - XXTEA 加密。
- `_cc_xxtea_decrypt` - XXTEA 解密。

## 使用示例

### 示例 1：XXTEA 加密
```c
#include <libcc/crypto/xxtea.h>
#include <stdio.h>

int main() {
    uint32_t key[4] = {0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210};
    uint32_t data[2] = {0x48656C6C, 0x6F215858}; // "Hello XX"
    _cc_xxtea_encrypt(data, 2, key);
    _cc_xxtea_decrypt(data, 2, key);
    printf("Decrypted: %s\n", (char*)data);
    return 0;
}
```

## 注意事项
1. **数据对齐**：输入数据必须是 32 位对齐的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。