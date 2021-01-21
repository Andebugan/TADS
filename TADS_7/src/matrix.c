#define __USE_MINGW_ANSI_STDIO 1
#include "../inc/matrix.h"

int allocate(matrix_t *mtx, size_t row_num, size_t column_num)
{
    mtx->row_num = row_num;
    mtx->column_num = column_num;
    mtx->value = malloc(mtx->row_num * sizeof(long *));

    if (mtx->value == NULL)
        return ALLOCATION_FAILURE;
    for (size_t i = 0; i < mtx->row_num; i++)
    {
        mtx->value[i] = malloc(mtx->column_num * sizeof(long));

        if (mtx->value[i] == NULL)
        {
            for (size_t k = 0; k < i; k++)
                free(mtx->value[k]);
            free(mtx->value);
            return ALLOCATION_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int zerofy(matrix_t *mtx)
{
    for (size_t i = 0; i < mtx->row_num; i++)
        for (size_t j = 0; j < mtx->column_num; j++)
            mtx->value[i][j] = 0;
    return EXIT_SUCCESS;
}

int make_adjacency(matrix_t *mtx, char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return ALLOCATION_FAILURE;

    size_t src, dst;
    while (1)
    {
        int scn = fscanf(file, "%zu %zu", &src, &dst);
        if (scn != 2)
        {
            if (scn < 0)
                break;
            else
                return EXIT_FAILURE;
        }
        if (src >= mtx->row_num || dst >= mtx->column_num)
            return INX_DONT_MATCH;

        mtx->value[src][dst] = 1;
        mtx->value[dst][src] = 1;
    }
    return EXIT_SUCCESS;
}

void copy(matrix_t src, matrix_t *dst)
{
    for (size_t i = 0; i < dst->row_num; i++)
        for (size_t j = 0; j < dst->column_num; j++)
            dst->value[i][j] = src.value[i][j];
}

void free_matrix(matrix_t *mtx)
{
    for (size_t i = 0; i < mtx->row_num; i++)
        free(mtx->value[i]);
    free(mtx->value);
}

void output(matrix_t mtx)
{
    for (size_t i = 0; i < mtx.row_num; i++)
    {
        for (size_t j = 0; j < mtx.column_num; j++)
            printf("%ld ", mtx.value[i][j]);
        printf("\n");
    }
}

void delete_row(matrix_t *mtx, size_t row)
{
    for (size_t i = row; i < mtx->row_num - 1; i++)
        for (size_t j = 0; j < mtx->column_num; j++)
            mtx->value[i][j] = mtx->value[i + 1][j];
    free(mtx->value[mtx->row_num - 1]);
    (mtx->row_num)--;
}

void transpose(matrix_t *src)
{
    matrix_t transposed;
    transposed.row_num = src->column_num;
    transposed.column_num = src->row_num;

    allocate(&transposed, transposed.row_num, transposed.column_num);

    for (size_t i = 0; i < src->row_num; i++)
        for (size_t j = 0; j < src->column_num; j++)
            transposed.value[j][i] = src->value[i][j];

    free_matrix(src);

    allocate(src, transposed.row_num, transposed.column_num);

    copy(transposed, src);
    free_matrix(&transposed);
}