#include "status_codes.h"
#include "sparse_matrix.h"
#include "test_checks.h"

int test_zip_start(void);
int test_zip_end(void);

int test_zip(void)
{
    BEGIN_TEST;

    SUB_TEST(test_zip_start);
    SUB_TEST(test_zip_end);

    END_TEST;
}

int test_zip_start(void)
{
    BEGIN_TEST;

    sp_mat_t mat;
    ASSIGN_MAT((&mat), 1, 3, {{ 2 C 1 C 10 }});

    mat.data_array[0] = 0;
    mat.data_array[1] = 0;

    sp_zip(&mat);

    EXPECT_MAT((&mat), 1, 3, {{ 0 C 0 C 10 }});

    END_TEST;
}

int test_zip_end(void)
{
    BEGIN_TEST;

    sp_mat_t mat;
    ASSIGN_MAT((&mat), 1, 3, {{ 2 C 1 C 10 }});

    mat.data_array[1] = 0;
    mat.data_array[2] = 0;

    sp_zip(&mat);

    EXPECT_MAT((&mat), 1, 3, {{ 2 C 0 C 0 }});

    END_TEST;
}
