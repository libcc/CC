# json.h 文档

## 文件概述
`json.h` 实现JSON解析与生成功能，兼容RFC8259标准。

## 核心功能
- `_cc_json_parse()` 解析JSON字符串
- `_cc_json_stringify()` 生成JSON字符串
- `_cc_json_get()` 获取字段值
- `_cc_json_set()` 设置字段值

## 使用示例
```c
#include <libcc/widgets/json.h>

_cc_json_t* obj = _cc_json_parse("{\"name\":\"value\"}");
const char* name = _cc_json_get(obj, "name");
_cc_json_set(obj, "new_field", _cc_json_new_number(123));
```

## 注意事项
- 解析失败返回NULL
- 需手动释放内存