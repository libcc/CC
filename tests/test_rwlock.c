#include <libcc/atomic.h>
#include <libcc/thread.h>
#include <assert.h>
#include <stdio.h>

#define NUM_THREADS 4
#define NUM_ITERATIONS 1000

int reader_thread(_cc_thread_t* thread, void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        _cc_rwlock_rlock(&lock);
        int value = shared_counter;
        _cc_rwlock_unlock(&lock);
    }
    return 0;
}


int writer_thread(_cc_thread_t* thread, void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        _cc_rwlock_wlock(&lock);
        shared_counter++;
        _cc_rwlock_unlock(&lock);
    }
    return 0;
}

void test_rwlock_concurrent_access() {
    _cc_lock_init(&lock);
    _cc_thread_t *threads[NUM_THREADS];

    // Create reader threads
    for (int i = 0; i < NUM_THREADS / 2; i++) {
        threads[i] = _cc_thread(reader_thread, "test_reader_thread", NULL);
    }

    // Create writer threads
    for (int i = NUM_THREADS / 2; i < NUM_THREADS; i++) {
        threads[i] = _cc_thread(writer_thread, "test_writer_thread", NULL);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        _cc_thread_join(threads[i], NULL);
    }

    assert(shared_counter == (NUM_THREADS / 2) * NUM_ITERATIONS);
    printf("Concurrent access test passed!\n");
}

void test_rwlock_downgrade() {
    _cc_lock_init(&lock);
    _cc_rwlock_wlock(&lock);
    _cc_rwlock_downgrade(&lock);
    assert(lock == 1);
    _cc_rwlock_unlock(&lock);
    printf("Downgrade test passed!\n");
}

void test_rwlock_single_thread() {
    _cc_lock_init(&lock);
    _cc_rwlock_rlock(&lock);
    assert(lock == 1);
    _cc_rwlock_unlock(&lock);
    assert(lock == 0);

    _cc_rwlock_wlock(&lock);
    assert(lock == _CC_RWLOCK_WLOCK_);
    _cc_rwlock_unlock(&lock);
    assert(lock == 0);
    printf("Single thread test passed!\n");
}

int main() {
    test_rwlock_single_thread();
    test_rwlock_downgrade();
    test_rwlock_concurrent_access();

    printf("All rwlock tests passed!\n");
    return 0;
}