#ifndef _SPARSE_MATRIX_H_
#define _SPARSE_MATRIX_H_

#include <stdio.h>

#include "matrix_common.h"

typedef struct sparse_matrix
{
    // число строк и столбцов матрицы
    mat_index_t rows;
    mat_index_t cols;

    // число ненулевых элементов (размер двух следующих массивов)
    mat_index_t nz_count;

    // вектор А содержащий значения ненулевых элементов
    mat_data_t *data_array; // размер = nz_amount

    // вектор JA содержащий номера столбцов для элементов вектора А
    mat_index_t *data_cols; // размер = nz_amount

    // вектор IA содержащий номера компонент с которых начинается описание строк матрицы
    mat_index_t *rows_array; // размер равен числу строк + 1 = rows + 1
} sp_mat_t;

// === функции ввода-вывода ===

// перевод из плотного представления
sp_mat_t sp_from_dense(mat_data_t **dense, mat_index_t rows, mat_index_t cols);

// ввод матрицы в виде матрицы
int sp_scanf(FILE *file, sp_mat_t *matrix);

// ввод матрицы в координатной форме
int sp_scanf_coord(FILE *file, sp_mat_t *matrix);

// ввод матрицы (с приглашением к вводу)
int sp_prompt(FILE *file, sp_mat_t *matrix);

// вывод матрицы в виде матрицы
int sp_print(FILE *file, sp_mat_t *matrix);

// вывод матрицы в координатной форме
int sp_print_coord(FILE *file, sp_mat_t *matrix);

// вывод матрицы в виде трёх векторов
int sp_print_raw(FILE *file, sp_mat_t *matrix);

// ==== функции обработки ====

// инициализирует нулевую матрицу для ручного заполнения
sp_mat_t sp_zero(mat_index_t rows, mat_index_t cols);

// вычисляет размер матрицы в байтах
size_t sp_calc_size(sp_mat_t *matrix);

// очищает память из-под матрицы
void sp_free_mat(sp_mat_t *matrix);

// получает конкретный элемент по индексам
mat_data_t sp_get(sp_mat_t *matrix, mat_index_t row, mat_index_t col);

// устанавливает конкретный элемент по индексам
void sp_set(sp_mat_t *matrix, mat_index_t row, mat_index_t col, mat_data_t value);

// удаление нулевых элементов из матрицы
void sp_zip(sp_mat_t *matrix);

// транспозиция матрицы на месте
void sp_transpose(sp_mat_t *matrix);

// умножение вектора-строки на матрицу (обычный метод)
// time - время выполнения функции (мс) (можно передать NULL)
int sp_mult_vector_slow(sp_mat_t *output, sp_mat_t *vector, sp_mat_t *matrix, float *time);

// умножение вектора-строки на матрицу (эффективный метод)
// time - время выполнения функции (мс) (можно передать NULL)
int sp_mult_vector_fast(sp_mat_t *output, sp_mat_t *vector, sp_mat_t *matrix, float *time);

#endif // _SPARSE_MATRIX_H_
