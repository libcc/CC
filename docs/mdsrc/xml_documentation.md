# xml.h 文档

## 文件概述
`xml.h` 实现轻量级XML解析器，支持DOM方式操作。

## 核心功能
- `_cc_xml_parse()` 解析XML字符串
- `_cc_xml_create()` 创建XML文档
- `_cc_xml_save()` 保存XML文件
- `_cc_xml_find()` 节点查找

## 使用示例
```c
#include <libcc/widgets/xml.h>

_cc_xml_doc_t* doc = _cc_xml_parse("<root><item>test</item></root>");
_cc_xml_node_t* node = _cc_xml_find(doc, "//item");
_cc_xml_save(doc, "output.xml");
```

## 注意事项
- 不支持DTD验证
- 大文件解析需注意内存消耗