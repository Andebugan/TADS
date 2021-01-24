#include "sparse_matrix.h"

int main(void)
{
    sp_mat_t mat;

    FILE *file = fopen("mat.txt", "rt");

    printf("scan status: %d\n", sp_scanf(file, &mat));

    sp_print_raw(stdout, &mat);
    printf("mat size: %lu bytes\n", sp_calc_size(&mat));

    sp_free_mat(&mat);

    return 0;
}
