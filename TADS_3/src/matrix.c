#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "sparse_matrix.h"

#define ASSERT_MAT(matrix)                                                                                                 \
    {                                                                                                                      \
        assert(matrix && "matrix cant be NULL");                                                                           \
        assert(matrix->data_array && matrix->data_cols && matrix->rows_array && "matrix is not initialized, but must be"); \
    }

// проверка на принадлежность ненулевого элемента с индексом index
// строке с индексом row в разреженной матрице matrix
#define INDEX_IN_ROW(matrix, row, index) (matrix->rows_array[row] <= (index) && (index) < matrix->rows_array[row + 1])

// проверка на принадлежность ненулевого элемента с индексом index
// столбцу с индексом col в разреженной матрице matrix
#define INDEX_IN_COL(matrix, col, index) (matrix->data_cols[index] == (col))

// проверка на принадлежность ненулевого элемента с индексом index
// строке с индексом row и столбцу с индексом col в разреженной матрице matrix
#define INDEX_IN_POS(matrix, row, col, index) (INDEX_IN_ROW(matrix, row, index) && INDEX_IN_COL(matrix, col, index))

// инициализирует нулевую матрицу для ручного заполнения
sp_mat_t sp_zero(mat_index_t rows, mat_index_t cols)
{
    sp_mat_t matrix;

    matrix.rows = rows;
    matrix.cols = cols;
    matrix.nz_count = 0;

    matrix.data_array = malloc(0);
    matrix.data_cols = malloc(0);
    matrix.rows_array = calloc(rows + 1, sizeof(mat_index_t));

    return matrix;
}

// очищает память из-под матрицы
void sp_free_mat(sp_mat_t *matrix)
{
    if (matrix->data_array != NULL)
    {
        free(matrix->data_array);
        matrix->data_array = NULL;

        free(matrix->data_cols);
        matrix->data_cols = NULL;

        free(matrix->rows_array);
        matrix->rows_array = NULL;
    }
}

// получает конкретный элемент по индексам
mat_data_t sp_get(sp_mat_t *matrix, mat_index_t row, mat_index_t col)
{
    ASSERT_MAT(matrix);
    mat_index_t nz_index = matrix->rows_array[row];
    mat_index_t nz_last = matrix->rows_array[row + 1];

    while (nz_index < nz_last && matrix->data_cols[nz_index] < col)
        nz_index++;

    if (nz_index < nz_last && matrix->data_cols[nz_index] == col)
        return matrix->data_array[nz_index];
    else
        return 0;
}

// устанавливает конкретный элемент по индексам
void sp_set(sp_mat_t *matrix, mat_index_t row, mat_index_t col, mat_data_t value)
{
    ASSERT_MAT(matrix);

    mat_index_t nz_index = matrix->rows_array[row];
    mat_index_t nz_last = matrix->rows_array[row + 1];

    while (nz_index < nz_last && matrix->data_cols[nz_index] < col)
        nz_index++;

    if (nz_index < nz_last && matrix->data_cols[nz_index] == col)
        matrix->data_array[nz_index] = value;
    else
    {
        matrix->nz_count++;
        matrix->data_array = realloc(matrix->data_array, matrix->nz_count * sizeof(mat_data_t));
        matrix->data_cols = realloc(matrix->data_cols, matrix->nz_count * sizeof(mat_data_t));

        mat_data_t temp_elem;
        mat_index_t temp_col;

        do
        {
            temp_elem = matrix->data_array[nz_index];
            temp_col = matrix->data_cols[nz_index];

            matrix->data_array[nz_index] = value;
            matrix->data_cols[nz_index] = col;

            value = temp_elem;
            col = temp_col;

            nz_index++;
        }
        while (nz_index < matrix->nz_count);

        // обновить список индексов строк
        while (row++ < matrix->rows)
            matrix->rows_array[row]++;
    }
}

// удаление нулевых элементов из матрицы
void sp_zip(sp_mat_t *matrix)
{
    ASSERT_MAT(matrix);

    mat_index_t z_count = 0;
    mat_index_t row = 0;
    for (mat_index_t nz_index = 0; nz_index + z_count < matrix->nz_count; nz_index++)
    {
        // при переходе на следующую строку удаляем из строки все нулевые
        while (matrix->rows_array[row + 1] <= nz_index + z_count)
            matrix->rows_array[++row] -= z_count; 

        if (matrix->data_array[nz_index] == 0) // наш клиент
            z_count++;

        matrix->data_array[nz_index] = matrix->data_array[nz_index + z_count];
        matrix->data_cols[nz_index] = matrix->data_cols[nz_index + z_count];
    }

    // при переходе на следующую строку удаляем из строки все нулевые
    while (row < matrix->rows + 1)
        matrix->rows_array[++row] -= z_count; 

    matrix->nz_count -= z_count;
    matrix->data_array = realloc(matrix->data_array, matrix->nz_count * sizeof(mat_data_t));
    matrix->data_cols = realloc(matrix->data_cols, matrix->nz_count * sizeof(mat_data_t));
}

// транспозиция матрицы на месте
void sp_transpose(sp_mat_t *matrix)
{
    sp_mat_t new_mat;
    new_mat.rows = matrix->cols;
    new_mat.cols = matrix->rows;
    new_mat.nz_count = matrix->nz_count;

    new_mat.data_array = malloc(new_mat.nz_count * sizeof(mat_data_t));
    new_mat.data_cols = malloc(new_mat.nz_count * sizeof(mat_index_t));
    new_mat.rows_array = malloc((new_mat.rows + 1) * sizeof(mat_index_t));

    mat_index_t new_nz_index = 0;
    new_mat.rows_array[0] = 0;
    for (mat_index_t col = 0; col < matrix->cols; col++)
    {
        for (mat_index_t nz_index = 0; nz_index < matrix->nz_count; nz_index++)
        {
            if (INDEX_IN_COL(matrix, col, nz_index))
            {
                // выяснить индекс строки в старой матрице
                mat_index_t row = 0;
                while (!INDEX_IN_ROW(matrix, row, nz_index))
                    row++;

                // записать этот элемент в транспонированную матрицу
                new_mat.data_array[new_nz_index] = matrix->data_array[nz_index];
                new_mat.data_cols[new_nz_index] = row;
                new_nz_index++;
            }
        }

        new_mat.rows_array[col + 1] = new_nz_index;
    }

    free(matrix->data_array);
    free(matrix->data_cols);
    free(matrix->rows_array);
    *matrix = new_mat;
}

// умножение вектора-строки на матрицу (обычный метод)
int sp_mult_vector_slow(sp_mat_t *output, sp_mat_t *vector, sp_mat_t *matrix)
{
    int status = SUCCESS;
    ASSERT_MAT(vector);
    ASSERT_MAT(matrix);

    assert(vector->rows == 1 && "given matrix is not row-vector");

    // проверка на возможность умножить вектор на матрицу
    if (vector->cols != matrix->rows)
        status = BAD_MAT_DIMS;
    else
    {
        // создать матрицу под произведение
        output->rows = 1;
        output->cols = matrix->cols;
        output->data_array = calloc(matrix->cols, sizeof(mat_data_t));
        output->data_cols = malloc(matrix->cols * sizeof(mat_index_t));
        output->rows_array = malloc(2 * sizeof(mat_index_t));

        // индекс текущего обрабатываемого элемента матрицы - он же индекс столбца
        for (mat_index_t col = 0; col < output->cols; col++)
        {
            // настройка индексов столбцов у элементов
            output->data_cols[col] = col;

            for (mat_index_t row = 0; row < matrix->rows; row++)
                output->data_array[col] += sp_get(vector, 0, row) * sp_get(matrix, row, col);
        }

        // начало и конец первой и единственной строки
        output->rows_array[0] = 0;
        output->rows_array[1] = output->cols + 1;

        sp_zip(output);
    }

    return status;
}

// умножение вектора-строки на матрицу (эффективный метод)
int sp_mult_vector_fast(sp_mat_t *output, sp_mat_t *vector, sp_mat_t *matrix)
{
    int status = SUCCESS;
    ASSERT_MAT(vector);
    ASSERT_MAT(matrix);

    assert(vector->rows == 1 && "given matrix is not row-vector");

    // проверка на возможность умножить вектор на матрицу
    if (vector->cols != matrix->rows)
        status = BAD_MAT_DIMS;
    else
    {
        // создать матрицу под произведение
        output->rows = 1;
        output->cols = matrix->cols;
        output->data_array = calloc(matrix->cols, sizeof(mat_data_t));
        output->data_cols = malloc(matrix->cols * sizeof(mat_index_t));
        output->rows_array = malloc(2 * sizeof(mat_index_t));

        // транспонировать исходную матрицу
        sp_transpose(matrix);

        // индекс текущего обрабатываемого элемента матрицы - он же индекс столбца
        for (mat_index_t nz_index = 0; nz_index < output->cols; nz_index++)
        {
            // настройка индексов столбцов у элементов
            output->data_cols[nz_index] = nz_index;

            // предустановка индексов для обхода
            mat_index_t nz_mat_index = matrix->rows_array[nz_index];
            mat_index_t nz_vec_index = 0;
            mat_index_t nz_mat_index_end = matrix->rows_array[nz_index + 1];
            mat_index_t nz_vec_index_end = vector->nz_count;

            mat_data_t sum = 0;

            // пока не дошли до конца хотябы одной из строк матриц
            while (nz_mat_index < nz_mat_index_end && nz_vec_index < nz_vec_index_end)
            {
                // находим ближайшую пару индексов nz_vec_index и nz_mat_index с одинаковыми индексами столбцов
                while (vector->data_cols[nz_vec_index] < matrix->data_cols[nz_mat_index])
                    nz_vec_index++;

                while (vector->data_cols[nz_vec_index] > matrix->data_cols[nz_mat_index])
                    nz_mat_index++;

                // если нашли, добавляем произведение к сумме и увеличиваем оба индекса
                if (nz_vec_index < nz_vec_index_end && nz_mat_index < nz_mat_index_end && vector->data_cols[nz_vec_index] == matrix->data_cols[nz_mat_index])
                    sum += vector->data_array[nz_vec_index] * matrix->data_array[nz_mat_index];

                nz_vec_index++;
                nz_mat_index++;
            }

            output->data_array[nz_index] = sum;
        }

        // начало и конец первой и единственной строки
        output->rows_array[0] = 0;
        output->rows_array[1] = output->cols + 1;

        sp_zip(output);

        // транспонировать исходную матрицу обратно
        sp_transpose(matrix);
    }

    return status;
}
