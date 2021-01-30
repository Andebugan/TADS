#include "status_codes.h"
#include "sparse_matrix.h"
#include "test_checks.h"

int test_mult_slow_1x3x3(void);
int test_mult_fast_1x3x3(void);
int test_mult_fast_1x4x3(void);

int test_matrix(void)
{
    BEGIN_TEST;

    SUB_TEST(test_mult_slow_1x3x3);
    SUB_TEST(test_mult_fast_1x3x3);
    SUB_TEST(test_mult_fast_1x4x3);

    END_TEST;
}

int test_mult_slow_1x3x3(void)
{
    BEGIN_TEST;

    sp_mat_t mat;
    sp_mat_t vec;
    sp_mat_t out;

    ASSIGN_MAT((&vec), 1, 3, {{ 4 C 5 C 0 }});

    ASSIGN_MAT((&mat), 3, 3, {
        { 2 C 1 C 0} C
        { 0 C 1 C 0} C
        { 0 C 0 C 3}
    });

    float time;
    int status = sp_mult_vector_slow(&out, &vec, &mat, &time);
    EXPECT_STATUS(status, SUCCESS);
    printf("slow time: %f ms\n", time);

    EXPECT_MAT((&out), 1, 3, {{ 8 C 9 C 0 }});

    sp_free_mat(&vec);
    sp_free_mat(&mat);
    sp_free_mat(&out);

    END_TEST;
}

int test_mult_fast_1x3x3(void)
{
    BEGIN_TEST;

    sp_mat_t mat;
    sp_mat_t vec;
    sp_mat_t out;

    ASSIGN_MAT((&vec), 1, 3, {{ 4 C 5 C 0 }});

    ASSIGN_MAT((&mat), 3, 3, {
        { 2 C 1 C 0} C
        { 0 C 1 C 0} C
        { 0 C 0 C 3}
    });

    float time;
    int status = sp_mult_vector_fast(&out, &vec, &mat, &time);
    EXPECT_STATUS(status, SUCCESS);
    printf("fast time: %f ms\n", time);

    EXPECT_MAT((&out), 1, 3, {{ 8 C 9 C 0 }});

    sp_free_mat(&vec);
    sp_free_mat(&mat);
    sp_free_mat(&out);

    END_TEST;
}

int test_mult_fast_1x4x3(void)
{
    BEGIN_TEST;

    sp_mat_t mat;
    sp_mat_t vec;
    sp_mat_t out;

    ASSIGN_MAT((&vec), 1, 4, {{ 0 C 2 C 0 C 3 }});

    ASSIGN_MAT((&mat), 4, 3, {
        { 2 C 0 C 0} C
        { 0 C 0 C 0} C
        { 0 C 0 C 1} C
        { 0 C 0 C 3}
    });

    float time;
    int status = sp_mult_vector_fast(&out, &vec, &mat, &time);
    EXPECT_STATUS(status, SUCCESS);
    printf("fast time: %f ms\n", time);

    printf("result mat:\n");
    sp_print_raw(stdout, &out);

    EXPECT_MAT((&out), 1, 3, {{ 0 C 0 C 9 }});

    sp_free_mat(&vec);
    sp_free_mat(&mat);
    sp_free_mat(&out);

    END_TEST;
}
