#include <libcc/array.h>
#include <assert.h>
#include <stdio.h>

void test_alloc_array() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    assert(array.limit == 10);
    assert(array.length == 0);
    assert(array.data != nullptr);
    _cc_free_array(&array);
    printf("Alloc array test passed!\n");
}

void test_realloc_array() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    assert(_cc_realloc_array(&array, 20) == true);
    assert(array.limit == 20);
    assert(array.length == 0);
    assert(array.data != nullptr);
    _cc_free_array(&array);
    printf("Realloc array test passed!\n");
}

void test_array_find() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    array.data[0] = (void *)1;
    array.length = 1;
    assert(_cc_array_find(&array, 0) == (void *)1);
    assert(_cc_array_find(&array, 1) == nullptr);
    _cc_free_array(&array);
    printf("Array find test passed!\n");
}

void test_array_push() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    size_t index = _cc_array_push(&array, (void *)1);
    assert(index == 0);
    assert(array.length == 1);
    assert(array.data[0] == (void *)1);
    _cc_free_array(&array);
    printf("Array push test passed!\n");
}

void test_array_pop() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    _cc_array_push(&array, (void *)1);
    assert(_cc_array_pop(&array) == (void *)1);
    assert(array.length == 0);
    _cc_free_array(&array);
    printf("Array pop test passed!\n");
}

void test_array_append() {
    _cc_array_t array1, array2;
    assert(_cc_alloc_array(&array1, 10) == true);
    assert(_cc_alloc_array(&array2, 10) == true);
    _cc_array_push(&array2, (void *)1);
    _cc_array_push(&array2, (void *)2);
    assert(_cc_array_append(&array1, &array2) == true);
    assert(array1.length == 2);
    assert(array1.data[0] == (void *)1);
    assert(array1.data[1] == (void *)2);
    _cc_free_array(&array1);
    _cc_free_array(&array2);
    printf("Array append test passed!\n");
}

void test_array_insert() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    assert(_cc_array_insert(&array, 0, (void *)1) == true);
    assert(array.length == 1);
    assert(array.data[0] == (void *)1);
    _cc_free_array(&array);
    printf("Array insert test passed!\n");
}

void test_array_set() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    assert(_cc_array_set(&array, 0, (void *)1) == true);
    assert(array.length == 1);
    assert(array.data[0] == (void *)1);
    assert(_cc_array_set(&array, 0, nullptr) == true);
    assert(array.length == 0);
    _cc_free_array(&array);
    printf("Array set test passed!\n");
}

void test_array_remove() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    _cc_array_push(&array, (void *)1);
    _cc_array_push(&array, (void *)2);
    assert(_cc_array_remove(&array, 0) == (void *)1);
    assert(array.length == 1);
    assert(array.data[0] == (void *)2);
    _cc_free_array(&array);
    printf("Array remove test passed!\n");
}

void test_array_cleanup() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    _cc_array_push(&array, (void *)1);
    assert(_cc_array_cleanup(&array) == true);
    assert(array.length == 0);
    _cc_free_array(&array);
    printf("Array cleanup test passed!\n");
}

void test_array_length() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    _cc_array_push(&array, (void *)1);
    assert(_cc_array_length(&array) == 1);
    _cc_free_array(&array);
    printf("Array length test passed!\n");
}

void test_array_begin_end() {
    _cc_array_t array;
    assert(_cc_alloc_array(&array, 10) == true);
    _cc_array_push(&array, (void *)1);
    _cc_array_push(&array, (void *)2);
    assert(_cc_array_begin(&array) == (void *)1);
    assert(_cc_array_end(&array) == (void *)2);
    _cc_free_array(&array);
    printf("Array begin/end test passed!\n");
}

int main() {
    test_alloc_array();
    test_realloc_array();
    test_array_find();
    test_array_push();
    test_array_pop();
    test_array_append();
    test_array_insert();
    test_array_set();
    test_array_remove();
    test_array_cleanup();
    test_array_length();
    test_array_begin_end();
    printf("All array tests passed!\n");
    return 0;
}