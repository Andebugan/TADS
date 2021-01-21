#ifndef _MATRIX_H_
#define _MATRIX_H_

#define ALLOCATION_FAILURE 1
#define INX_DONT_MATCH 2

#include <stdio.h>
#include <stdlib.h>

struct matrix
{
    long **value;
    size_t column_num;
    size_t row_num;
};
typedef struct matrix matrix_t;

int allocate(matrix_t *mtx, size_t row_num, size_t column_num);

int zerofy(matrix_t *mtx);

int make_adjacency(matrix_t *mtx, char *filename);

void copy(matrix_t src, matrix_t *dst);

void free_matrix(matrix_t *mtx);

void remove_edge(matrix_t *mtx, size_t row);

void output(matrix_t mtx);

void delete_row(matrix_t *mtx, size_t row);

void transpose(matrix_t *src);

int export_to_dot(char *filename, matrix_t graph, size_t deleted);

#endif // _MATRIX_H_