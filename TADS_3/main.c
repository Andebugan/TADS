#include "sparse_matrix.h"

int main(void)
{
    sp_mat_t mat;

    FILE *file = fopen("mat.txt", "rt");

    printf("%d\n", sp_scanf(file, &mat));

    return 0;
}
