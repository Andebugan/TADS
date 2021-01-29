#include "status_codes.h"
#include "sparse_matrix.h"
#include "colors.h"
 
#define ASSERT_MAT(matrix)                                                                                                 \
    {                                                                                                                      \
        assert(matrix && "matrix cant be NULL");                                                                           \
        assert(matrix->data_array && matrix->data_cols && matrix->rows_array && "matrix is not initialized, but must be"); \
    }

#define SAFE_FPRINTF(...)                              \
    if (status == SUCCESS && fprintf(__VA_ARGS__) < 0) \
        status = BAD_FPRINTF;

// Проверка на принадлежность ненулевого элемента с индексом index
// строке с индексом row в разреженной матрице matrix
#define INDEX_IN_ROW(matrix, row, index) (matrix->rows_array[row] <= (index) && (index) < matrix->rows_array[row + 1])

// Проверка на принадлежность ненулевого элемента с индексом index
// столбцу с индексом col в разреженной матрице matrix
#define INDEX_IN_COL(matrix, col, index) (matrix->data_cols[index] == (col))

// Проверка на принадлежность ненулевого элемента с индексом index
// строке с индексом row и столбцу с индексом col в разреженной матрице matrix
#define INDEX_IN_POS(matrix, row, col, index) (INDEX_IN_ROW(matrix, row, index) && INDEX_IN_COL(matrix, col, index))

// Перевод из плотного представления
sp_mat_t sp_from_dense(mat_data_t **dense, mat_index_t rows, mat_index_t cols)
{
    // Инициализация матрицы нулями
    sp_mat_t matrix = sp_zero(rows, cols);

    for (mat_index_t row = 0; row < rows; row++)
        for (mat_index_t col = 0; col < cols; col++)
            if (dense[row][col] != 0)
            // Устанавливаем элемент в "обычную" матрицу по индексам row и col
                sp_set(&matrix, row, col, dense[row][col]);

    return matrix;
}

// Считывание строки чисел и возвращение динамического массива
// если значение cols было равно 0, то устанавливает в кол-во чисел в строке
// иначе - если оно не равно кол-ву чисел в строке - вернёт NULL
static mat_data_t *parse_line(char *line, mat_index_t *cols)
{
    int status = SUCCESS;
    // Выделения памяти и инциализация динамического массива
    mat_data_t *array = malloc(sizeof(mat_data_t));
    mat_index_t count = 0;

    // Разбиваем строку на разделителям с помощью функции strtok
    char *token = strtok(line, " \t\n");
    // При успешном разбиении считываем строку
    while (status == SUCCESS && token != NULL)
    {
        char c;
        // Ввод строки
        if (sscanf(token, MAT_DATA_SCN_FMT "%c", array + count, &c) != 1)
            status = BAD_FSCANF;
        else
        {   
            // Перевыделение памяти
            array = realloc(array, (++count + 1) * sizeof(mat_data_t));
            token = strtok(NULL, " \t\n");
        }
    }

    // Очистка массива при ошибке в выделении памяти
    if (status != SUCCESS || (*cols != 0 && *cols != count))
    {
        free(array);
        array = NULL;
    }

    *cols = count;
    return array;
}

// Ввод матрицы в виде матрицы
int sp_scanf(FILE *file, sp_mat_t *matrix)
{
    int status = SUCCESS;
    // Инициализация переменных для записи количества строк, столбцов и плотной матрицы
    mat_index_t rows = 0;
    mat_index_t cols = 0;
    mat_data_t **dense_matrix = NULL;
    // Буферная строка
    char *line = NULL;
    size_t line_size = 0;

    // Пока ввод успешный - записываем строку в матрицу
    while (status == SUCCESS && getline(&line, &line_size, file) >= 0)
    {
        // Перевыделение памяти
        dense_matrix = realloc(dense_matrix, (rows + 1) * sizeof(mat_data_t *));
        // Преобразование строки в массив
        dense_matrix[rows] = parse_line(line, &cols);
        if (dense_matrix[rows] == NULL)
            status = BAD_FSCANF;
        else
            rows++;
    }
    // Освобождаем буфер
    free(line);

    if (cols == 0 || rows == 0)
        status = BAD_FILE;
    // Переводим матрицу из плотного представления в вид хранения разреженной матрицы
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

// Ввод вектора в виде матрицы
int sp_scanf_vec(FILE *file, sp_mat_t *matrix)
{
    int status = SUCCESS;
    // Инициализация переменных для записи количества строк, столбцов и плотной матрицы
    mat_index_t rows = 0;
    mat_index_t cols = 0;
    mat_data_t **dense_matrix = NULL;
    // Буферная строка
    char *line = NULL;
    size_t line_size = 0;

    // Пока ввод успешный - записываем строку в матрицу
    if (getline(&line, &line_size, file) >= 0)
    {
        // Перевыделение памяти
        dense_matrix = realloc(dense_matrix, (rows + 1) * sizeof(mat_data_t *));
        // Преобразование строки в массив
        dense_matrix[rows] = parse_line(line, &cols);
        if (dense_matrix[rows] == NULL)
            status = BAD_FSCANF;
        else
            rows++;
    }
    // Освобождаем буфер
    free(line);

    if (cols == 0 || rows == 0)
        status = BAD_FILE;
    // Переводим матрицу из плотного представления в вид хранения разреженной матрицы
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


// Ввод матрицы в координатной форме
int sp_scanf_coord(FILE *file, sp_mat_t *matrix);

// Ввод матрицы (с приглашением к вводу)
int sp_prompt(FILE *file, sp_mat_t *matrix);

// Вывод матрицы в виде матрицы
int sp_print(FILE *file, sp_mat_t *matrix)
{
    //ASSERT_MAT(matrix);
    int status = SUCCESS;
    if (matrix == NULL)
    {
        printf(RED "<!> Внимание, матрица не инициализирована\n" RESET);
        status = ZERO_MAT;
    }
    mat_index_t nz_index = 0;
    // Для каждого элемента матрицы
    for (mat_index_t row = 0; status == SUCCESS && row < matrix->rows; row++)
    {
        for (mat_index_t col = 0; col < matrix->cols; col++)
        {
            // Разделитель
            if (col > 0)
                SAFE_FPRINTF(file, " ");

            if (nz_index < matrix->nz_count &&            // проверка на наличие невыведенных ненулевых элементов
                INDEX_IN_POS(matrix, row, col, nz_index)) // проверка на соответствие текущей позиции
            {
                // Выводим в файл данные при отсутсвии ненулевых элементов и соответсвие текущей позиции элемента
                SAFE_FPRINTF(file, MAT_DATA_PRN_FMT, matrix->data_array[nz_index]);
                nz_index++;
            }
            else
                // Иначе выводим 0
                SAFE_FPRINTF(file, MAT_DATA_PRN_FMT, (mat_data_t)0);

            if (status != SUCCESS)
                break;
        }
        // Переходим на новую строку
        SAFE_FPRINTF(file, "\n");

        if (status != SUCCESS)
            break;
    }
    if (status == ZERO_MAT)
        status = SUCCESS;
    return status;
}

// Вывод вектор в виде вектора
int sp_print_vec(FILE *file, sp_mat_t *matrix)
{
    //ASSERT_MAT(matrix);
    int status = SUCCESS;
    if (matrix == NULL)
    {
        printf(RED "<!> Внимание, вектор не инициализирован\n" RESET);
        status = ZERO_MAT;
    }
    if (matrix->rows != 1)
    {
        printf(RED "<!> Внимание, вектор инкорректен или не инициализирован\n" RESET);
        status = ZERO_MAT;
    }
    mat_index_t nz_index = 0;
    for (mat_index_t col = 0; status == SUCCESS && col < matrix->cols; col++)
    {
        // Разделитель
        if (col > 0)
            SAFE_FPRINTF(file, " ");

        if (nz_index < matrix->nz_count &&            // проверка на наличие невыведенных ненулевых элементов
            INDEX_IN_POS(matrix, 0, col, nz_index)) // проверка на соответствие текущей позиции
        {
            // Выводим в файл данные при отсутсвии ненулевых элементов и соответсвие текущей позиции элемента
            SAFE_FPRINTF(file, MAT_DATA_PRN_FMT, matrix->data_array[nz_index]);
            nz_index++;
        }
        else
            // Иначе выводим 0
            SAFE_FPRINTF(file, MAT_DATA_PRN_FMT, (mat_data_t)0);

        if (status != SUCCESS)
            break;
    }
    // Переходим на новую строку
    SAFE_FPRINTF(file, "\n");
    if (status == ZERO_MAT)
        status = SUCCESS;
    
    return status;
}

// Вывод матрицы в координатной форме
int sp_print_coord(FILE *file, sp_mat_t *matrix)
{
    // Проверка матрицы
    //ASSERT_MAT(matrix);
    int status = SUCCESS;
    if (matrix == NULL)
    {
        printf(RED "<!> Внимание, матрица не инициализирована\n" RESET);
        status = ZERO_MAT;
    }
    mat_index_t row = 0;
    for (mat_index_t nz_index = 0; status == SUCCESS && nz_index < matrix->nz_count; nz_index++)
    {
        // Переход на следующую строку при прочтении всех чисел из текущей строки
        while (!INDEX_IN_ROW(matrix, row, nz_index))
            row++;

        // Отпределяем столбец
        mat_index_t col = matrix->data_cols[nz_index];
        // Определяем элемент
        mat_data_t element = matrix->data_array[nz_index];

        // Вывод в файл
        SAFE_FPRINTF(file,
            MAT_INDEX_FMT " " MAT_INDEX_FMT " " MAT_DATA_PRN_FMT "\n",
            row, col, element
        );
    }
    if (status == ZERO_MAT)
        status = SUCCESS;
    return status;
}

// Вывод вектора в координатной форме
int sp_print_coord_vec(FILE *file, sp_mat_t *matrix)
{
    // Проверка матрицы
    //ASSERT_MAT(matrix);
    int status = SUCCESS;
    if (matrix == NULL)
    {
        printf(RED "<!> Внимание, вектор не инициализирован\n" RESET);
        status = ZERO_MAT;
    }
    if (matrix->rows != 1)
    {
        printf(RED "<!> Внимание, вектор инкорректен\n" RESET);
        status = ZERO_MAT;
    }
    for (mat_index_t nz_index = 0; status == SUCCESS && nz_index < matrix->nz_count; nz_index++)
    {
        // Отпределяем столбец
        mat_index_t col = matrix->data_cols[nz_index];
        // Определяем элемент
        mat_data_t element = matrix->data_array[nz_index];

        // Вывод в файл
        SAFE_FPRINTF(file,
            MAT_INDEX_FMT " " MAT_DATA_PRN_FMT "\n",
            col, element
        );
    }
    if (status == ZERO_MAT)
        status = SUCCESS;
    return status;
}

// Вывод матрицы в виде трёх векторов
int sp_print_raw(FILE *file, sp_mat_t *matrix)
{
    int status = SUCCESS;
    if (matrix == NULL)
    {
        printf(RED "<!> Внимание, матрица не инициализирована\n" RESET);
        status = ZERO_MAT;
    }
    else
        SAFE_FPRINTF(file, "A: [");
    // Вывод вектора данных
    for (mat_index_t nz_index = 0; status == SUCCESS &&  nz_index < matrix->nz_count; nz_index++)
    {
        if (nz_index > 0)
            SAFE_FPRINTF(file, ", ");

        SAFE_FPRINTF(file, "%d", (int)matrix->data_array[nz_index]);
    }

    if (status == SUCCESS)
        SAFE_FPRINTF(file, "]\nJA: [");
    // Вывод вектора номеров столбцов
    for (mat_index_t nz_index = 0; status == SUCCESS &&  nz_index < matrix->nz_count; nz_index++)
    {
        if (nz_index > 0)
            SAFE_FPRINTF(file, ", ");

        SAFE_FPRINTF(file, "%u", (unsigned int)matrix->data_cols[nz_index]);
    }

    if (status == SUCCESS)
        SAFE_FPRINTF(file, "]\nIA: [");
    // Вывод вектора хранения номеров первых элементов по строкам
    for (mat_index_t row = 0; status == SUCCESS &&  row < matrix->rows + 1; row++)
    {
        if (row > 0)
            SAFE_FPRINTF(file, ", ");

        SAFE_FPRINTF(file, "%u", (unsigned int)matrix->rows_array[row]);
    }

    if (status == SUCCESS)
        SAFE_FPRINTF(file, "]\n");
    if (status == ZERO_MAT)
        status = SUCCESS;
    return status;
}
