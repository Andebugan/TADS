#include "sparse_matrix.h"
#include "status_codes.h"
#include "test_checks.h"

int test_scanf_empty(void);
int test_scanf_1x1(void);

int test_matrix_io(void)
{
    BEGIN_TEST;

    SUB_TEST(test_scanf_empty);
    SUB_TEST(test_scanf_1x1);

    END_TEST;
}

int test_scanf_empty(void)
{
    BEGIN_TEST;

    FILE *file = fopen("unit/data/empty.txt", "rt");
    sp_mat_t mat;

    int status = sp_scanf(file, &mat);
    EXPECT_STATUS_NOT(status, SUCCESS);

    fclose(file);

    END_TEST;
}

int test_scanf_1x1(void)
{
    BEGIN_TEST;

    FILE *file = fopen("unit/data/1x1.txt", "rt");
    sp_mat_t mat;

    int status = sp_scanf(file, &mat);
    EXPECT_STATUS(status, SUCCESS);

    EXPECT_MAT((&mat), 1, 1, {{ 53 }});

    sp_free_mat(&mat);
    fclose(file);

    END_TEST;
}
