# ini.h 文档

## 文件概述
`ini.h` 提供INI配置文件解析功能，支持节(section)、键值对(key-value)的读写操作。

## 核心功能
- `_cc_ini_load()` 加载INI文件
- `_cc_ini_save()` 保存INI文件
- `_cc_ini_get()` 获取键值
- `_cc_ini_set()` 设置键值

## 使用示例
```c
#include <libcc/widgets/ini.h>

_cc_ini_t* config = _cc_ini_load("config.ini");
const char* value = _cc_ini_get(config, "section", "key");
_cc_ini_set(config, "section", "key", "new_value");
_cc_ini_save(config, "config.ini");
```

## 注意事项
- 线程不安全，需自行加锁
- 文件编码应为UTF-8