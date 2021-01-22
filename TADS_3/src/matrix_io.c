#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "status_codes.h"
#include "sparse_matrix.h"

#define ASSERT_MAT(matrix)                                                                                                 \
    {                                                                                                                      \
        assert(matrix && "matrix cant be NULL");                                                                           \
        assert(matrix->data_array && matrix->data_cols && matrix->rows_array && "matrix is not initialized, but must be"); \
    }

#define SAFE_FPRINTF(...)                              \
    if (status == SUCCESS && fprintf(__VA_ARGS__) < 0) \
        status = BAD_FPRINTF;

#define MAT_INDEX_FMT "%4hu"
#define MAT_DATA_PRN_FMT "% 6lld"
#define MAT_DATA_SCN_FMT "%lld"

// проверка на принадлежность ненулевого элемента с индексом index
// строке с индексом row в разреженной матрице matrix
#define INDEX_IN_ROW(matrix, row, index) (matrix->rows_array[row] <= (index) && (index) < matrix->rows_array[row + 1])

// проверка на принадлежность ненулевого элемента с индексом index
// столбцу с индексом col в разреженной матрице matrix
#define INDEX_IN_COL(matrix, col, index) (matrix->data_cols[index] == (col))

// проверка на принадлежность ненулевого элемента с индексом index
// строке с индексом row и столбцу с индексом col в разреженной матрице matrix
#define INDEX_IN_POS(matrix, row, col, index) (INDEX_IN_ROW(matrix, row, index) && INDEX_IN_COL(matrix, col, index))

// перевод из плотного представления
sp_mat_t sp_from_dense(mat_data_t **dense, mat_index_t rows, mat_index_t cols)
{
    sp_mat_t matrix = sp_zero(rows, cols);

    for (mat_index_t row = 0; row < rows; row++)
        for (mat_index_t col = 0; col < cols; col++)
            if (dense[row][col] != 0)
                sp_set(&matrix, row, col, dense[row][col]);

    return matrix;
}

// считывает строку чисел и возвращает динамический массив
// если значение cols было равно 0, то устанавливает в кол-во чисел в строке
// иначе - если оно не равно кол-ву чисел в строке - вернёт NULL
static mat_data_t *parse_line(char *line, mat_index_t *cols)
{
    int status = SUCCESS;

    mat_data_t *array = malloc(sizeof(mat_data_t));
    mat_index_t count = 0;

    // пройти по строке функцией strtok
    char *token = strtok(line, " \t\n");
    while (status == SUCCESS && token != NULL)
    {
        char c;

        if (sscanf(token, MAT_DATA_SCN_FMT "%c", array + count, &c) != 1)
            status = BAD_FSCANF;
        else
        {
            array = realloc(array, (++count + 1) * sizeof(mat_data_t));
            token = strtok(NULL, " \t\n");
        }
    }

    if (status != SUCCESS || (*cols != 0 && *cols != count))
    {
        free(array);
        array = NULL;
    }

    *cols = count;
    return array;
}

// ввод матрицы в виде матрицы
int sp_scanf(FILE *file, sp_mat_t *matrix)
{
    int status = SUCCESS;

    mat_index_t rows = 0;
    mat_index_t cols = 0;
    mat_data_t **dense_matrix = NULL;

    char *line = NULL;
    size_t line_size = 0;

    while (status == SUCCESS && getline(&line, &line_size, file) >= 0)
    {
        dense_matrix = realloc(dense_matrix, (rows + 1) * sizeof(mat_data_t *));
        dense_matrix[rows] = parse_line(line, &cols);
        if (dense_matrix[rows] == NULL)
            status = BAD_FSCANF;
        else
            rows++;
    }

    free(line);

    if (cols == 0 || rows == 0)
        status = BAD_FILE;

    if (status == SUCCESS)
        *matrix = sp_from_dense(dense_matrix, rows, cols);

    if (dense_matrix != NULL)
    {
        for (mat_index_t row = 0; row < rows; row++)
            free(dense_matrix[row]);
        free(dense_matrix);
    }

    return status;
}

// ввод матрицы в координатной форме
int sp_scanf_coord(FILE *file, sp_mat_t *matrix);

// ввод матрицы (с приглашением к вводу)
int sp_prompt(FILE *file, sp_mat_t *matrix);

// вывод матрицы в виде матрицы
int sp_print(FILE *file, sp_mat_t *matrix)
{
    ASSERT_MAT(matrix);
    int status = SUCCESS;

    mat_index_t nz_index = 0;
    for (mat_index_t row = 0; row < matrix->rows; row++)
    {
        for (mat_index_t col = 0; col < matrix->cols; col++)
        {
            if (col > 0)
                SAFE_FPRINTF(file, " ");

            if (nz_index < matrix->nz_count &&            // проверка на наличие невыведенных ненулевых элементов
                INDEX_IN_POS(matrix, row, col, nz_index)) // проверка на соответствие текущей позиции
            {
                SAFE_FPRINTF(file, MAT_DATA_PRN_FMT, matrix->data_array[nz_index]);
                nz_index++;
            }
            else
                SAFE_FPRINTF(file, MAT_DATA_PRN_FMT, (mat_data_t)0);

            if (status != SUCCESS)
                break;
        }

        SAFE_FPRINTF(file, "\n");

        if (status != SUCCESS)
            break;
    }

    return status;
}

// вывод матрицы в координатной форме
int sp_print_coord(FILE *file, sp_mat_t *matrix)
{
    ASSERT_MAT(matrix);
    int status = SUCCESS;

    mat_index_t row = 0;
    for (mat_index_t nz_index = 0; status == SUCCESS && nz_index < matrix->nz_count; nz_index++)
    {
        // перескакиваем на следующую строку, когда напечатали все числа из текущей
        while (!INDEX_IN_ROW(matrix, row, nz_index))
            row++;

        mat_index_t col = matrix->data_cols[nz_index];
        mat_data_t element = matrix->data_array[nz_index];

        SAFE_FPRINTF(file,
            MAT_INDEX_FMT " " MAT_INDEX_FMT " " MAT_DATA_PRN_FMT "\n",
            row, col, element
        );
    }

    return status;
}

// вывод матрицы в виде трёх векторов
int sp_print_raw(FILE *file, sp_mat_t *matrix)
{
    int status = SUCCESS;

    SAFE_FPRINTF(file, "A: [");

    for (mat_index_t nz_index = 0; nz_index < matrix->nz_count; nz_index++)
    {
        if (nz_index > 0)
            SAFE_FPRINTF(file, ", ");

        SAFE_FPRINTF(file, "%d", (int)matrix->data_array[nz_index]);
    }

    SAFE_FPRINTF(file, "]\nJA: [");

    for (mat_index_t nz_index = 0; nz_index < matrix->nz_count; nz_index++)
    {
        if (nz_index > 0)
            SAFE_FPRINTF(file, ", ");

        SAFE_FPRINTF(file, "%u", (unsigned int)matrix->data_cols[nz_index]);
    }

    SAFE_FPRINTF(file, "]\nIA: [");

    for (mat_index_t row = 0; row < matrix->rows + 1; row++)
    {
        if (row > 0)
            SAFE_FPRINTF(file, ", ");

        SAFE_FPRINTF(file, "%u", (unsigned int)matrix->rows_array[row]);
    }

    SAFE_FPRINTF(file, "]\n");

    return status;
}
