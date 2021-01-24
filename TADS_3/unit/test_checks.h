#ifndef _TEST_CHECKS_H_
#define _TEST_CHECKS_H_

#include "sparse_matrix.h"

#define C ,
#define ASSIGN_MAT(matrix, _rows, _cols, dense_static)             \
    {                                                              \
        *matrix = sp_zero(_rows, _cols);                           \
        mat_data_t dense_mat[_rows][_cols] = dense_static;         \
        for (mat_index_t row = 0; row < _rows; row++)              \
            for (mat_index_t col = 0; col < _cols; col++)          \
                if (dense_mat[row][col] != 0)                      \
                    sp_set(matrix, row, col, dense_mat[row][col]); \
    }

#define EXPECT_MAT(matrix, _rows, _cols, dense_static)                                                                         \
    {                                                                                                                          \
        int bad_bit = 0;                                                                                                       \
        mat_data_t dense_mat[_rows][_cols] = dense_static;                                                                     \
        if (matrix->rows != _rows || matrix->cols != _cols)                                                                    \
        {                                                                                                                      \
            fprintf(stderr, "bad matrix dims. Expected (%d, %d), got (%hu, %hu)\n", _rows, _cols, matrix->rows, matrix->cols); \
            bad_bit = 1;                                                                                                       \
        }                                                                                                                      \
        else                                                                                                                   \
        {                                                                                                                      \
            for (mat_index_t row = 0; row < _rows; row++)                                                                      \
            {                                                                                                                  \
                for (mat_index_t col = 0; col < _cols; col++)                                                                  \
                {                                                                                                              \
                    if (sp_get(matrix, row, col) != dense_mat[row][col])                                                       \
                    {                                                                                                          \
                        fprintf(stderr, "bad element at pos (%u, %u): ", (unsigned int)row, (unsigned int)col);                \
                        fprintf(stderr, "expected %d, got %d\n", (int)dense_mat[row][col], (int)sp_get(matrix, row, col));     \
                        bad_bit = 1;                                                                                           \
                    }                                                                                                          \
                }                                                                                                              \
            }                                                                                                                  \
        }                                                                                                                      \
        if (bad_bit == 1)                                                                                                      \
            fails++;                                                                                                           \
    }

#define EXPECT_STATUS(actual, expected)                                      \
    if (actual != expected)                                                  \
    {                                                                        \
        fprintf(stderr, "status = %d, but expected %d\n", actual, expected); \
        fails++;                                                             \
    }

#define EXPECT_STATUS_NOT(actual, expected)                    \
    if (actual == expected)                                    \
    {                                                          \
        fprintf(stderr, "status = %d not expected\n", actual); \
        fails++;                                               \
    }

#define BEGIN_TEST \
    int fails = 0

#define END_TEST                                                  \
    if (fails > 0)                                                \
        fprintf(stderr, "%s: fails %d times\n", __func__, fails); \
    return fails

#define SUB_TEST(func) \
    fails += func()

#endif // _TEST_CHECKS_H_
