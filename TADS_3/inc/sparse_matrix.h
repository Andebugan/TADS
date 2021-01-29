#ifndef _SPARSE_MATRIX_H_
#define _SPARSE_MATRIX_H_

#define MAT_INDEX_FMT "%4hu"
#define MAT_DATA_PRN_FMT "% 6lld"
#define MAT_DATA_SCN_FMT "%lld"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "matrix_common.h"

// Структура для хранения разреженной матрицы
typedef struct sparse_matrix
{
    // Число строк и столбцов матрицы
    mat_index_t rows;
    mat_index_t cols;

    // Число ненулевых элементов (размер двух следующих массивов)
    mat_index_t nz_count;

    // Вектор А содержащий значения ненулевых элементов
    mat_data_t *data_array; // размер = nz_amount

    // Вектор JA содержащий номера столбцов для элементов вектора А
    mat_index_t *data_cols; // размер = nz_amount

    // Вектор IA содержащий номера компонент с которых начинается описание строк матрицы
    mat_index_t *rows_array; // размер равен числу строк + 1 = rows + 1
} sp_mat_t;

// === функции ввода-вывода ===

// Перевод из плотного представления
sp_mat_t sp_from_dense(mat_data_t **dense, mat_index_t rows, mat_index_t cols);

// Ввод матрицы в виде матрицы
int sp_scanf(FILE *file, sp_mat_t *matrix);

// Ввод вектора в виде вектора
int sp_scanf_vec(FILE *file, sp_mat_t *matrix);

// Ввод матрицы в координатной форме
int sp_scanf_coord(FILE *file, sp_mat_t *matrix);

// Ввод матрицы (с приглашением к вводу)
int sp_prompt(FILE *file, sp_mat_t *matrix);

// Вывод матрицы в виде матрицы
int sp_print(FILE *file, sp_mat_t *matrix);

// Вывод вектора на экран
int sp_print_vec(FILE *file, sp_mat_t *matrix);

// Вывод вектора в координатном виде
int sp_print_coord_vec(FILE *file, sp_mat_t *matrix);

// Вывод матрицы в координатной форме
int sp_print_coord(FILE *file, sp_mat_t *matrix);

// Вывод матрицы в виде трёх векторов
int sp_print_raw(FILE *file, sp_mat_t *matrix);

// ==== функции обработки ====

// Инициализация матрицы для ручного заполнения (матрица нулей заданного размера)
sp_mat_t sp_zero(mat_index_t rows, mat_index_t cols);

// Вычисление размера памяти под матрицу в байтах
size_t sp_calc_size(sp_mat_t *matrix);

// Очистка памяти из-под матрицы
void sp_free_mat(sp_mat_t *matrix);

// Получение конкретного элемента по индексу
mat_data_t sp_get(sp_mat_t *matrix, mat_index_t row, mat_index_t col);

// Установление конкретного элемента по индексу
void sp_set(sp_mat_t *matrix, mat_index_t row, mat_index_t col, mat_data_t value);

// Удаление нулевых элементов из матрицы
void sp_zip(sp_mat_t *matrix);

// Транспозиция матрицы на месте
void sp_transpose(sp_mat_t *matrix);

// Умножение вектора-строки на матрицу (обычный метод)
// time - время выполнения функции (мс) (можно передать NULL)
int sp_mult_vector_slow(sp_mat_t *output, sp_mat_t *vector, sp_mat_t *matrix, float *time);

// Умножение вектора-строки на матрицу (эффективный метод)
// time - время выполнения функции (мс) (можно передать NULL)
int sp_mult_vector_fast(sp_mat_t *output, sp_mat_t *vector, sp_mat_t *matrix, float *time);

#endif // _SPARSE_MATRIX_H_
