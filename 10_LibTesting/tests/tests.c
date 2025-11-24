#include <check.h>
#include <stdint.h>
#include <math.h>
#include "grow.h"

START_TEST(test_init)
{
    float *a = NULL;
    ck_assert_ptr_null(a);
    ck_assert_uint_eq(buf_size(a), 0);
    ck_assert_uint_eq(buf_capacity(a), 0);
    buf_free(a);
}
END_TEST

START_TEST(test_push_pop)
{
    float *a = NULL;
    buf_push(a, 1.5f);
    ck_assert_uint_eq(buf_size(a), 1);
    ck_assert(fabs(a[0] - 1.5f) < 0.0001f);
    
    float val = buf_pop(a);
    ck_assert(fabs(val - 1.5f) < 0.0001f);
    ck_assert_uint_eq(buf_size(a), 0);
    buf_free(a);
}
END_TEST

START_TEST(test_grow)
{
    float *a = NULL;
    buf_grow(a, 10);
    ck_assert_uint_eq(buf_capacity(a), 10);
    ck_assert_uint_eq(buf_size(a), 0);
    
    buf_trunc(a, 5);
    ck_assert_uint_eq(buf_capacity(a), 5);
    buf_free(a);
}
END_TEST

START_TEST(test_clear)
{
    float *a = NULL;
    buf_push(a, 1.0f);
    buf_push(a, 2.0f);
    ck_assert_uint_eq(buf_size(a), 2);
    
    buf_clear(a);
    ck_assert_uint_eq(buf_size(a), 0);
    ck_assert_ptr_nonnull(a);
    buf_free(a);
}
END_TEST

START_TEST(test_multi_push)
{
    long *ai = NULL;
    for (int i = 0; i < 100; i++)
        buf_push(ai, i);
    
    ck_assert_uint_eq(buf_size(ai), 100);
    for (int i = 0; i < 100; i++)
        ck_assert_int_eq(ai[i], i);
    buf_free(ai);
}
END_TEST

START_TEST(test_free)
{
    float *a = NULL;
    buf_push(a, 1.0f);
    buf_free(a);
    ck_assert_ptr_null(a);
}
END_TEST

Suite *grow_suite(void)
{
    Suite *s = suite_create("grow");
    TCase *tc = tcase_create("core");
    
    tcase_add_test(tc, test_init);
    tcase_add_test(tc, test_push_pop);
    tcase_add_test(tc, test_grow);
    tcase_add_test(tc, test_clear);
    tcase_add_test(tc, test_multi_push);
    tcase_add_test(tc, test_free);
    
    suite_add_tcase(s, tc);
    return s;
}

int main(void)
{
    SRunner *sr = srunner_create(grow_suite());
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
