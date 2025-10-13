#include <libcc/sql.h>
#include <assert.h>
#include <stdio.h>

void test_sql_delegate_init_mysql() {
    _cc_sql_delegate_t delegate;
    assert(_cc_init_mysql(&delegate) == true);
    printf("test_sql_delegate_init_mysql passed\n");
}

void test_sql_delegate_init_sqlsvr() {
    _cc_sql_delegate_t delegate;
    assert(_cc_init_sqlsvr(&delegate) == true);
    printf("test_sql_delegate_init_sqlsvr passed\n");
}

void test_sql_delegate_init_sqlite() {
    _cc_sql_delegate_t delegate;
    assert(_cc_init_sqlite(&delegate) == true);
    printf("test_sql_delegate_init_sqlite passed\n");
}

void test_sql_delegate_init_oci8() {
    _cc_sql_delegate_t delegate;
    assert(_cc_init_oci8(&delegate) == true);
    printf("test_sql_delegate_init_oci8 passed\n");
}

int main() {
    test_sql_delegate_init_mysql();
    test_sql_delegate_init_sqlsvr();
    test_sql_delegate_init_sqlite();
    test_sql_delegate_init_oci8();
    return 0;
}
