# uuid.h 文档

## 文件概述
`uuid.h` 是一个实现 UUID（通用唯一标识符）生成和转换的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. UUID 类型
定义了 `_cc_uuid_t` 类型，用于存储 UUID（16 字节）。

### 2. UUID 生成
提供了函数 `_cc_uuid`，用于生成 UUID。

### 3. UUID 转换
支持以下转换函数：
- `_cc_uuid_to_bytes` - 将字符串转换为 UUID。
- `_cc_uuid_lower` - 将 UUID 转换为小写字符串。
- `_cc_uuid_upper` - 将 UUID 转换为大写字符串。

## 使用示例

### 示例 1：生成 UUID
```c
#include <libcc/uuid.h>
#include <stdio.h>

int main() {
    _cc_uuid_t uuid;
    _cc_uuid(&uuid);
    tchar_t buf[33];
    _cc_uuid_lower(&uuid, buf, sizeof(buf));
    printf("UUID: %s\n", buf);
    return 0;
}
```

## 注意事项
1. **线程安全**：函数 `_cc_uuid` 是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。