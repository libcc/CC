# atomic.h 文档

## 文件概述
`atomic.h` 是一个实现原子操作的头文件，支持多线程环境下的原子读写。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 原子类型
定义了以下类型：
- `_cc_atomic32_t` - 32 位原子整数。
- `_cc_atomic64_t` - 64 位原子整数。

### 2. 原子操作
提供了以下函数：
- `_cc_atomic32_add` - 原子加法。
- `_cc_atomic32_set` - 原子赋值。
- `_cc_atomic32_cas` - 原子比较交换。

### 3. 锁机制
提供了以下函数：
- `_cc_lock` - 获取锁。
- `_cc_unlock` - 释放锁。

## 使用示例

### 示例 1：原子计数器
```c
#include <libcc/atomic.h>
#include <stdio.h>

int main() {
    _cc_atomic32_t counter = 0;
    _cc_atomic32_add(&counter, 1);
    printf("Counter: %d\n", _cc_atomic32_load(&counter));
    return 0;
}
```

## 注意事项
1. **线程安全**：原子操作本身是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。