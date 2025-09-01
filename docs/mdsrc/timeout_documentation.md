# timeout.h 文档

## 文件概述
`timeout.h` 提供定时器管理功能，支持单次和周期性定时器。

## 核心功能
- `_cc_timeout_add()` 添加定时器
- `_cc_timeout_remove()` 移除定时器
- `_cc_timeout_process()` 处理到期定时器

## 使用示例
```c
#include <libcc/widgets/timeout.h>

void callback(void* arg) {
    printf("Timeout!\n");
}

_cc_timeout_add(1000, callback, NULL); // 1秒后触发
while(1) {
    _cc_timeout_process();
}
```

## 注意事项
- 需在主循环中调用process
- 定时精度依赖系统时钟