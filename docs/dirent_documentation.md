# dirent.h 文档

## 文件概述
`dirent.h` 是一个实现目录操作的头文件，支持跨平台的目录遍历。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 目录操作
提供了以下函数：
- `opendir` - 打开目录。
- `readdir` - 读取目录项。
- `closedir` - 关闭目录。

### 2. 文件类型检测
提供了函数 `_cc_isdir`，用于检测路径是否为目录。

## 使用示例

### 示例 1：遍历目录
```c
#include <libcc/dirent.h>
#include <stdio.h>

int main() {
    DIR *dir = opendir(".");
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            printf("%s\n", entry->d_name);
        }
        closedir(dir);
    }
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：支持 Windows 和 Unix 平台。
2. **线程安全**：函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。