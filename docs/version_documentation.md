# version.h 文档

## 文件概述
`version.h` 是一个定义 `libcc` 项目版本信息的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 版本号定义
定义了 `libcc` 的主版本号、次版本号和微版本号：
- `_CC_MAJOR_VERSION_` - 主版本号（当前为 3）。
- `_CC_MINOR_VERSION_` - 次版本号（当前为 0）。
- `_CC_MICRO_VERSION_` - 微版本号（当前为 0）。

### 2. 版本字符串
定义了版本字符串 `_CC_VERSION_`，格式为 `"主版本号.次版本号.微版本号"`（当前为 `"3.0.0"`）。

### 3. 版本号转换
提供了宏 `_CC_VERSIONNUM`，用于将版本号转换为一个数值，格式为 `主版本号 * 1000000 + 次版本号 * 1000 + 微版本号`。

### 4. 获取版本号
提供了函数 `_cc_get_version`，用于获取当前 `libcc` 的版本号数值。

## 使用示例

### 示例 1：获取版本号
```c
#include <libcc/version.h>
#include <stdio.h>

int main() {
    printf("libcc version: %s\n", _CC_VERSION_);
    printf("Version number: %d\n", _cc_get_version());
    return 0;
}
```

## 注意事项
1. **版本兼容性**：版本号遵循语义化版本规范（SemVer），主版本号变更表示不兼容的 API 修改。
2. **线程安全**：函数 `_cc_get_version` 是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。