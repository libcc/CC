#include <libcc/thread.h>
#include <assert.h>
#include <stdio.h>

static _cc_thread_local_t int thread_local_value = 0;
static int global_value = 0;
static int thread_callback(void *args) {
    int *value = (int *)args;
    (*value)++;
    thread_local_value++;
    global_value++;
    printf("Global value:%d!\nThread local value:%d!\n", global_value,thread_local_value);
    return 0;
}

void test_thread_creation() {
    int value = 0;
    _cc_thread_t *thread = _cc_thread(thread_callback, "test_thread", &value);
    assert(thread != NULL);
    _cc_wait_thread(thread, NULL);
    assert(value == 1);
    printf("Thread creation test passed!\n");
}

void test_thread_start() {
    int value = 0;
    bool_t result = _cc_thread_start(thread_callback, "test_thread", &value);
    assert(result == true);
    // Wait for the thread to complete (detached threads may need a delay)
    _cc_sleep(100);
    assert(value == 1);
    printf("Thread start test passed!\n");
}

void test_thread_with_stacksize() {
    int value = 0;
    _cc_thread_t *thread = _cc_thread_with_stacksize(thread_callback, "test_thread", 1024, &value);
    assert(thread != NULL);
    _cc_wait_thread(thread, NULL);
    assert(value == 1);
    printf("Thread with stack size test passed!\n");
}

void test_thread_detach() {
    int value = 0;
    _cc_thread_t *thread = _cc_thread(thread_callback, "test_thread", &value);
    assert(thread != NULL);
    _cc_detach_thread(thread);
    // Wait for the thread to complete (detached threads may need a delay)
    _cc_sleep(100);
    assert(value == 1);
    printf("Thread detach test passed!\n");
}

void test_thread_get_id() {
    _cc_thread_t *thread = _cc_thread(thread_callback, "test_thread", NULL);
    assert(thread != NULL);
    size_t thread_id = _cc_get_thread_id(thread);
    assert(thread_id != 0);
    _cc_wait_thread(thread, NULL);
    printf("Thread get ID test passed!\n");
}

int main() {
    test_thread_creation();
    test_thread_start();
    test_thread_with_stacksize();
    test_thread_detach();
    test_thread_get_id();

    printf("All thread tests passed!\n");
    return true;
}