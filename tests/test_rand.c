#include <libcc/rand.h>
#include <assert.h>
#include <stdio.h>

void test_srand_and_rand() {
    _cc_srand(42);
    int32_t val1 = _cc_rand(100);
    int32_t val2 = _cc_rand(100);
    assert(val1 >= 0 && val1 < 100);
    assert(val2 >= 0 && val2 < 100);
    assert(val1 != val2);
    printf("srand and rand test passed!\n");
}

void test_randf() {
    float32_t val = _cc_randf();
    assert(val >= 0.0f && val < 1.0f);
    printf("randf test passed!\n");
}

void test_rand_bits() {
    int32_t val = _cc_rand_bits();
    assert(val != 0);
    printf("rand_bits test passed!\n");
}

void test_rand_r() {
    uint64_t state = 42;
    int32_t val = _cc_rand_r(&state, 100);
    assert(val >= 0 && val < 100);
    printf("rand_r test passed!\n");
}

void test_randf_r() {
    uint64_t state = 42;
    float32_t val = _cc_randf_r(&state);
    assert(val >= 0.0f && val < 1.0f);
    printf("randf_r test passed!\n");
}

void test_random_bytes() {
    byte_t buf[16];
    _cc_random_bytes(buf, sizeof(buf));
    bool_t all_zero = true;
    for (size_t i = 0; i < sizeof(buf); i++) {
        if (buf[i] != 0) {
            all_zero = false;
            break;
        }
    }
    assert(!all_zero);
    printf("random_bytes test passed!\n");
}

void test_randomf32() {
    float32_t val = _cc_randomf32(1.0f, 10.0f);
    assert(val >= 1.0f && val < 10.0f);
    printf("randomf32 test passed!\n");
}

void test_randomf64() {
    float64_t val = _cc_randomf64(1.0, 10.0);
    assert(val >= 1.0 && val < 10.0);
    printf("randomf64 test passed!\n");
}

void test_random32() {
    uint32_t val = _cc_random32(1, 100);
    assert(val >= 1 && val < 100);
    printf("random32 test passed!\n");
}

void test_random64() {
    uint64_t val = _cc_random64(1, 100);
    assert(val >= 1 && val < 100);
    printf("random64 test passed!\n");
}

void test_get_probability() {
    _cc_prd_t prd;
    _cc_calculate_prd(&prd, 50.0);
    int32_t result = _cc_get_probability(&prd, 10);
    assert(result == 0 || result == 1);
    printf("get_probability test passed!\n");
}

int main() {
    test_srand_and_rand();
    test_randf();
    test_rand_bits();
    test_rand_r();
    test_randf_r();
    test_random_bytes();
    test_randomf32();
    test_randomf64();
    test_random32();
    test_random64();
    test_get_probability();
    return 0;
}