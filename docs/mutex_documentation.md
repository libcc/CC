# mutex.h 文档

## 文件概述
`mutex.h` 是一个实现互斥锁和同步原语的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 互斥锁
提供了以下函数：
- `_cc_alloc_mutex` - 创建互斥锁。
- `_cc_free_mutex` - 释放互斥锁。
- `_cc_mutex_lock` - 加锁。
- `_cc_mutex_unlock` - 解锁。

### 2. 信号量
提供了以下函数：
- `_cc_alloc_semaphore` - 创建信号量。
- `_cc_free_semaphore` - 释放信号量。
- `_cc_semaphore_wait` - 等待信号量。
- `_cc_semaphore_post` - 释放信号量。

## 使用示例

### 示例 1：使用互斥锁
```c
#include <libcc/mutex.h>

int main() {
    _cc_mutex_t* mutex = _cc_alloc_mutex();
    _cc_mutex_lock(mutex);
    // 临界区代码
    _cc_mutex_unlock(mutex);
    _cc_free_mutex(mutex);
    return 0;
}
```

## 注意事项
1. **线程安全**：正确使用同步原语保证线程安全。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。