# sql.h 文档

## 文件概述
`sql.h` 提供SQLite数据库操作接口，封装常用CRUD操作。

## 核心功能
- `_cc_sql_open()` 打开数据库
- `_cc_sql_exec()` 执行SQL语句
- `_cc_sql_query()` 查询数据
- `_cc_sql_close()` 关闭连接

## 使用示例
```c
#include <libcc/widgets/sql.h>

_cc_sql_t* db = _cc_sql_open("test.db");
_cc_sql_exec(db, "CREATE TABLE users(id INTEGER PRIMARY KEY, name TEXT)");
_cc_sql_query_t* result = _cc_sql_query(db, "SELECT * FROM users");
_cc_sql_close(db);
```

## 注意事项
- 需链接sqlite3库
- 错误处理需检查返回值