#include "menu.h"
#include "status_codes.h"
#include "sparse_matrix.h"
#include "matrix_common.h"

// Очиста потока ввода
void clean_input_stream()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

// Считывание введёной строки с номером опции
static int get_option(void)
{
    // Размер строки для ввода опции
    const int line_size = 16;
    // Статическая строка для ввода опции
    char line[line_size];
    // Инициализация опции
    int option = -1;
    // Считывание опции с клавиатуры
    fgets(line, line_size, stdin);
    sscanf(line, "%d", &option);

    return option;
}   

// Вывод главного меню
static void show_main_menu(void)
{
    printf(CYAN "МЕНЮ:\n" RESET
        YELLOW
        "  Опции ввода:\n"
        RESET
        "  1. Ввести матрицу координатным методом с консоли\n"
        "  2. Ввести матрицу в матричной форме из файла\n"
        "  3. Ввести матрицу в координатной форме из файла\n"
        "  4. Сгенерировать разреженную матрицу заданной размерности и процентом заполнения\n"
        YELLOW
        "  Опции вывода\n"
        RESET
        "  5. Вывести матрицу в матричном виде\n"
        "  6. Вывести матрицу в координатной форме\n"
        "  7. Вывести матрицу в форме трёх векторов\n"
        YELLOW
        "  Опции умножения матрицы\n"
        RESET
        "  8. Ввести вектор-строку в координатном виде с консоли\n"
        "  9. Ввести вектор-строку в координатном виде из файла\n"
        "  10. Ввести вектор-строку в виде массива из файла\n"
        "  11. Умножить вектор-строку на матрицу применяя стандартный метод\n"
        "  12. Умножить вектор-строку на матрицу применяя эффективный метод\n"
        "  13. Сравнить время выполнения операций и объем памяти при использовании\n"
        "     этих 2-х алгоритмов при различном проценте заполнения матриц.\n"
        "  14. Вывести вектор на экран\n"
        YELLOW
        "  0. Выйти\n" RESET
        CYAN "> " RESET);
}

// Генерация матрицы 
int sp_mat_gen(void)
{
    int status = SUCCESS;

    return status;
}

// Ввод матрицы в координатном виде
int get_matrix_coord(sp_mat_t *matrix)
{
    int status = SUCCESS;
    int rows = 0;
    int cols = 0;
    int elem_col = 0;
    int err_count = 0;
    int exit = 0;

    printf(CYAN "Формат ввода:\n" RESET
    YELLOW
    "Сначала вводится размерность матрицы и количество вводимых элементов в формате {i} {j} {col}\n"
    "Размерность - целые числа в интервале [1, ");
    printf("%i", MAX_MATR_SIZE);
    printf("]\nКоличество элементов - целое число в интервале [1, ");
    printf("%i", MAX_MATR_SIZE * MAX_MATR_SIZE);
    printf("]\nЭлементы вводятся в формате {i} {j} {элемент}\n"
    "Элементы не обязательно вводятся в том порядке,\n"
    "в котором они расположены а матрице\n"
    "При введении элемента который уже есть в матрице\n"
    "на экран будет выведено предупреждение о перезаписи элемента\n"
    "Все элементы - целые числа\n"
    RESET);

    printf (YELLOW "Введите размерность матрицы: " RESET);
    if (fscanf(stdin, "%d %d %d", &rows, &cols, &elem_col) != 3 || ((rows > MAX_MATR_SIZE || rows <= 0)\
     || (cols > MAX_MATR_SIZE || cols <= 0) || (elem_col <= 0) || (elem_col > rows * cols)))
    {
        status = BAD_MAT_DIMS;
        printf(RED "<!> Ошибка, размеры матрицы не соответствуют условию\n" RESET);
    }
    else
    {
        *matrix = sp_zero(rows, cols);
        if (matrix != NULL)
        {
            printf(GREEN "Матрица успешно инициализирована\n" RESET);
            printf (YELLOW "Введите элементы в формате {i} {j} {элемент}:\n" RESET);
        }
        else
        {
            printf(RED "<!> Ошибка при выделении памяти, попробуйте ещё раз\n" RESET);
            status = FAILURE;
        }
    }

    // Считывание элементов
    for (int i = 0; status == SUCCESS && i < elem_col &&  exit == 0; i++)
    {
        int row = 0;
        int col = 0;
        int elem = 0;
        if (scanf("%d %d %d", &row, &col, &elem) != 3 || ((row > rows || row < 0)\
         || (col > cols || col < 0)))
        {
            printf(RED "<!> Ошибка, элемент введен некорректно, попробуйте ещё раз\n" RESET);
            i--;
            err_count++;
            if (err_count > 5)
            {
                printf(RED "<!> Ошибка, элемент введен некорректно 5 раз, ввод завершается\n" RESET);
                status = BAD_ELEMS;
                exit = 1;
                sp_free_mat(matrix);
            }
        }
        else
        {
            err_count = 0;
            sp_set(matrix, row, col, elem);
        }
    }
    if (status == SUCCESS)
        printf(GREEN "Ввод матрицы успешно завершён\n" RESET);
    clean_input_stream();
    return status;
}

// Ввод вектора в координатном виде
int get_matrix_coord_vec(sp_mat_t *matrix)
{
    int status = SUCCESS;
    int rows = 1;
    int cols = 0;
    int elem_col = 0;
    int err_count = 0;
    int exit = 0;

    printf(CYAN "Формат ввода:\n" RESET
    YELLOW
    "Сначала вводится размерность вектора и количество вводимых элементов в формате {j} {col}\n"
    "Размерность - целое число в интервале [1, ");
    printf("%i", MAX_MATR_SIZE);
    printf("]\nКоличество элементов - целое число в интервале [1, ");
    printf("%i", MAX_MATR_SIZE * MAX_MATR_SIZE);
    printf("]\nЭлементы вводятся в формате {j} {элемент}\n"
    "Элементы не обязательно вводятся в том порядке,\n"
    "в котором они расположены а векторе\n"
    "При введении элемента который уже есть в векторе\n"
    "на экран будет выведено предупреждение о перезаписи элемента\n"
    "Все элементы - целые числа\n"
    RESET);

    printf (YELLOW "Введите размерность матрицы: " RESET);
    if (fscanf(stdin, "%d %d", &cols, &elem_col) != 2 && ((cols > MAX_MATR_SIZE || cols <= 0)\
     || (elem_col <= 0) || (elem_col > cols)))
    {
        status = BAD_MAT_DIMS;
        printf(RED "<!> Ошибка, размеры вектора не соответствуют условию\n" RESET);
    }
    else
    {
        *matrix = sp_zero(rows, cols);
        if (matrix != NULL)
        {
            printf(GREEN "Вектор успешно инициализирован\n" RESET);
            printf (YELLOW "Введите элементы в формате {j} {элемент}:\n" RESET);
        }
        else
        {
            printf(RED "<!> Ошибка при выделении памяти, попробуйте ещё раз\n" RESET);
            status = FAILURE;
        }
    }

    // Считывание элементов
    for (int i = 0; status == SUCCESS && i < elem_col &&  exit == 0; i++)
    {
        int row = 0;
        int col = 0;
        int elem = 0;
        if (scanf("%d %d", &col, &elem) != 2 || ((row > rows || row < 0)\
         || (col > cols || col < 0)))
        {
            printf(RED "<!> Ошибка, элемент введен некорректно, попробуйте ещё раз\n" RESET);
            i--;
            err_count++;
            if (err_count > 5)
            {
                printf(RED "<!> Ошибка, элемент введен некорректно 5 раз, ввод завершается\n" RESET);
                status = BAD_ELEMS;
                exit = 1;
                sp_free_mat(matrix);
            }
        }
        else
        {
            err_count = 0;
            sp_set(matrix, row, col, elem);
        }
    }
    if (status == SUCCESS)
        printf(GREEN "Ввод вектора успешно завершён\n" RESET);
    clean_input_stream();
    return status;
}

// Ввод вектора в координатном виде из файла
int get_matrix_coord_file_vec(sp_mat_t *matrix)
{
    int status = SUCCESS;
    int rows = 1;
    int cols = 0;
    int elem_col = 0;

    FILE *file;
    char fname[20];
    printf(CYAN "Формат ввода:\n" 
    YELLOW "Введите название файла, в котором содержится вектор в координатном формате\n"
    "Длина имени файла не должна превышать %d символов: " RESET, MAX_FNAME);
    if (scanf("%s", fname) != 1)
        status = BAD_FNAME;
    else
    {
        file = fopen(fname, "r");
        if (file == NULL)
        {
            status = BAD_FNAME;
            printf(RED "<!> Имя файла некорректно, попробуйте ещё раз\n" RESET);
        }
    }

    if (fscanf(file, "%d %d", &cols, &elem_col) != 2\
     || (cols <= 0) || (elem_col > rows * cols))
    {
        status = BAD_MAT_DIMS;
        printf(RED "<!> Ошибка, размеры вектора не соответствуют условию\n" RESET);
    }
    else
    {
        *matrix = sp_zero(rows, cols);
        if (matrix != NULL)
        {
            printf(GREEN "Вектор успешно инициализирован\n" RESET);
        }
        else
        {
            printf(RED "<!> Ошибка при выделении памяти, попробуйте ещё раз\n" RESET);
            status = FAILURE;
        }
    }

    // Считывание элементов
    for (int i = 0; status == SUCCESS && i < elem_col; i++)
    {
        int row = 0;
        int col = 0;
        int elem = 0;
        if (fscanf(file, "%d %d", &col, &elem) != 2 || (col > cols || col < 0))
            status = BAD_FSCANF;
        else
            sp_set(matrix, row, col, elem);
    }
    if (status == SUCCESS)
        printf(GREEN "Считывание вектора успешно завершено\n" RESET);
    else if (status == BAD_FSCANF)
    {
        printf(RED "<!> Ошибка, ввод элементов из файла был прекращён по причине ошибки в формате\n" RESET);
    }
    else
        status = SUCCESS;
    clean_input_stream();
    return status;
}

// Ввод матрицы в координатном виде из файла
int get_matrix_coord_file(sp_mat_t *matrix)
{
    int status = SUCCESS;
    int rows = 0;
    int cols = 0;
    int elem_col = 0;

    FILE *file;
    char fname[20];
    printf(CYAN "Формат ввода:\n" 
    YELLOW "Введите название файла, в котором содержится матрица в координатном формате\n"
    "Длина имени файла не должна превышать %d символов: " RESET, MAX_FNAME);
    if (scanf("%s", fname) != 1)
        status = BAD_FNAME;
    else
    {
        file = fopen(fname, "r");
        if (file == NULL)
        {
            status = BAD_FNAME;
            printf(RED "<!> Имя файла некорректно, попробуйте ещё раз\n" RESET);
        }
    }

    if (fscanf(file, "%d %d %d", &rows, &cols, &elem_col) != 3 || (rows <= 0)\
     || (cols <= 0) || (elem_col > rows * cols))
    {
        status = BAD_MAT_DIMS;
        printf(RED "<!> Ошибка, размеры матрицы не соответствуют условию\n" RESET);
    }
    else
    {
        *matrix = sp_zero(rows, cols);
        if (matrix != NULL)
        {
            printf(GREEN "Матрица успешно инициализирована\n" RESET);
        }
        else
        {
            printf(RED "<!> Ошибка при выделении памяти, попробуйте ещё раз\n" RESET);
            status = FAILURE;
        }
    }

    // Считывание элементов
    for (int i = 0; status == SUCCESS && i < elem_col; i++)
    {
        int row = 0;
        int col = 0;
        int elem = 0;
        if (fscanf(file, "%d %d %d", &row, &col, &elem) != 3 || ((row > rows || row < 0)\
         || (col > cols || col < 0)))
            status = BAD_FSCANF;
        else
            sp_set(matrix, row, col, elem);
    }
    if (status == SUCCESS)
        printf(GREEN "Считывание матрицы успешно завершено\n" RESET);
    else if (status == BAD_FSCANF)
    {
        printf(RED "<!> Ошибка, ввод элементов из файла был прекращён по причине ошибки в формате\n" RESET);
    }
    else
        status = SUCCESS;
    clean_input_stream();
    return status;
}

// Ввод матрицы в виде матрицы из файла
int get_matr_file_matr(sp_mat_t *matrix)
{
    int status = SUCCESS;
    FILE *file;
    char fname[20];
    printf(CYAN "Формат ввода:\n" 
    YELLOW "Введите название файла, в котором содержится матрица в матричном формате\n"
    "Длина имени файла не должна превышать %d символов: " RESET, MAX_FNAME);
    if (scanf("%s", fname) != 1)
        status = BAD_FNAME;
    else
    {
        file = fopen(fname, "r");
        if (file == NULL)
        {
            status = BAD_FNAME;
            printf(RED "<!> Имя файла некорректно, попробуйте ещё раз\n" RESET);
        }
        else
            status = sp_scanf(file, matrix);
    }
    if (status == SUCCESS)
        printf(GREEN "Ввод матрицы успешно завершён\n" RESET);
    else if (status != BAD_FNAME)
    {
        status = SUCCESS;
        printf(RED "<!> При считывании матрицы возникла ошибка, попробуйте ещё раз\n" RESET);
    }
    else
        status = SUCCESS;
    clean_input_stream();
    return status;
}

// Ввод вектора в виде массива из файла
int get_matr_file_matr_vec(sp_mat_t *matrix)
{
    int status = SUCCESS;
    FILE *file;
    char fname[20];
    printf(CYAN "Формат ввода:\n" 
    YELLOW "Введите название файла, в котором содержится вектор в виде массива\n"
    "Длина имени файла не должна превышать %d символов: " RESET, MAX_FNAME);
    if (scanf("%s", fname) != 1)
        status = BAD_FNAME;
    else
    {
        file = fopen(fname, "r");
        if (file == NULL)
        {
            status = BAD_FNAME;
            printf(RED "<!> Имя файла некорректно, попробуйте ещё раз\n" RESET);
        }
        else
            status = sp_scanf_vec(file, matrix);
    }
    if (status == SUCCESS)
        printf(GREEN "Ввод вектора успешно завершён\n" RESET);
    else if (status != BAD_FNAME)
    {
        status = SUCCESS;
        printf(RED "<!> При считывании вектора возникла ошибка, попробуйте ещё раз\n" RESET);
    }
    else
        status = SUCCESS;
    clean_input_stream();
    return status;
}

// Меню вывода вектора в консоль
int print_vector_to_console(sp_mat_t *matrix)
{
    int status = SUCCESS;
    printf(YELLOW 
    "Выберите вид вывода вектора:\n"
    "1 - в виде массива\n"
    "2 - в координатном виде\n"
    CYAN "> " RESET);
    int option = get_option();
    switch (option)
    {
    case 1:
        if (matrix->nz_count > 0 && (matrix->cols > MAX_CONSOLE_SIZE || matrix->rows > MAX_CONSOLE_SIZE))
        {
            printf(RED "<!> Внимание, размерность вектора не позволяет вывести её в виде матрицы в консоль\n" RESET);
        }
        else
            status = sp_print_vec(stdout, matrix);
        break;
    case 2:
        printf(CYAN "Формат вывода:\n"
            YELLOW
            "Первый столбец - j, второй - значение элемента\n"
            RESET);
        status = sp_print_coord_vec(stdout, matrix);
        break;
    // Ошибка ввода опции
    default:
        printf(RED "<!> Неверная опция.\n\n" RESET);
        break;
    }
    return status;
}

// Меню вывода вектора в файл
int print_vector_to_file(sp_mat_t *matrix)
{
    int status = SUCCESS;
    FILE *file;
    char fname[20];
    printf(CYAN "Формат вывода:\n" 
    YELLOW "Введите название файла, в который вы хотите вывести вектор\n"
    "Длина имени файла не должна превышать %d символов: " RESET, MAX_FNAME);
    if (scanf("%s", fname) != 1)
        status = BAD_FNAME;
    else
    {
        file = fopen(fname, "w+");
        if (file == NULL)
        {
            status = BAD_FNAME;
            printf(RED "<!> Имя файла некорректно, попробуйте ещё раз\n" RESET);
        }
    }
    clean_input_stream();
    if (status == SUCCESS)
    {
        printf(YELLOW 
        "Выберите вид вывода вектора:\n"
        "1 - в виде массива\n"
        "2 - в координатном виде\n"
        CYAN "> " RESET);
        int option = get_option();
        switch (option)
        {
        case 1:
            if (matrix->nz_count > 0 && (matrix->cols > MAX_CONSOLE_SIZE || matrix->rows > MAX_CONSOLE_SIZE))
            {
                printf(RED "<!> Внимание, размерность вектора не позволяет вывести её в виде матрицы в консоль\n" RESET);
            }
            else
                status = sp_print_vec(file, matrix);
            break;
        case 2:
            printf(CYAN "Формат вывода:\n"
                YELLOW
                "Первый столбец - j, второй - значение элемента\n"
                RESET);
            status = sp_print_coord_vec(file, matrix);
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }
        if (status == SUCCESS)
            printf(GREEN "Вывод произошёл успешно\n" RESET);
    }
    return status;
}

// Умножение стандартное
int standart_mult(sp_mat_t *matrix, sp_mat_t *vector)
{
    int status = SUCCESS;
    float time = 0;
    sp_mat_t output;
    status = sp_mult_vector_slow(&output, vector, matrix, &time);
    if (status == SUCCESS)
    {
        printf(GREEN "Умножение прошло успешно\n"
        YELLOW 
        "Время умножения: %f\n" 
        "Выберите способ вывода результата умножения:\n" 
        "1 - Вывод в консоль\n" 
        "2 - Вывод в файл\n" CYAN "> " RESET, time);
        int option = get_option();
        switch (option)
        {
        // Вывод в консоль
        case 1:
            status = print_vector_to_console(&output);
            break;
        // Вывод в файл
        case 2:
            status = print_vector_to_file(&output);
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }
    }
    else
    {
        printf(RED "<!> Внимание, при умножении возникла ошибка\n" RESET);
        status = SUCCESS;
    }
    return status;
}

// Умножение быстрое
int fast_mult(sp_mat_t *matrix, sp_mat_t *vector)
{
    int status = SUCCESS;
    float time = 0;
    sp_mat_t output;
    status = sp_mult_vector_fast(&output, vector, matrix, &time);
    if (status == SUCCESS)
    {
        printf(GREEN "Умножение прошло успешно\n"
        YELLOW 
        "Время умножения: %f\n" 
        "Выберите способ вывода результата умножения:\n" 
        "1 - Вывод в консоль\n" 
        "2 - Вывод в файл\n" CYAN "> " RESET, time);
        int option = get_option();
        switch (option)
        {
        // Вывод в консоль
        case 1:
            status = print_vector_to_console(&output);
            break;
        // Вывод в файл
        case 2:
            status = print_vector_to_file(&output);
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }
    }
    else
    {
        printf(RED "<!> Внимание, при умножении возникла ошибка\n" RESET);
        status = SUCCESS;
    }
    return status;
}

// Сравнение умножения
int mult_diff(sp_mat_t *matrix, sp_mat_t *vector)
{
    int status = SUCCESS;
    float time_f = 0;
    float time_s = 0;
    sp_mat_t output_f;
    sp_mat_t output_s;
    status = sp_mult_vector_fast(&output_f, vector, matrix, &time_f);
    status = sp_mult_vector_slow(&output_s, vector, matrix, &time_s);
    if (status == SUCCESS)
    {
        printf(GREEN "Умножение прошло успешно\n"
        YELLOW 
        "Время быстрого умножения: %f\n" 
        "Время медленного умножения: %f\n" 
        "Выберите способ вывода результата умножения:\n" 
        "1 - Вывод в консоль\n" 
        "2 - Вывод в файл\n" CYAN "> " RESET, time_f, time_s);
        int option = get_option();
        switch (option)
        {
        // Вывод в консоль
        case 1:
            printf(YELLOW "Медленный способ:\n" RESET);
            status = print_vector_to_console(&output_s);
            printf(YELLOW "Быстрый способ:\n" RESET);
            status = print_vector_to_console(&output_f);
            break;
        // Вывод в файл
        case 2:
            status = print_vector_to_file(&output_s);
            status = print_vector_to_file(&output_f);
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }
    }
    else
    {
        printf(RED "<!> Внимание, при умножении возникла ошибка\n" RESET);
        status = SUCCESS;
    }
    return status;
}


// Функция работы с программой
int menu_loop(void)
{
    int status = 0;
    bool exit = false;
    sp_mat_t matrix;
    sp_mat_t vector;
    printf(YELLOW "Лабораторная работа №3\n" RESET);
    // Menu loop
    while(!exit)
    {
        printf(YELLOW "---------------------------------\n" RESET);
        show_main_menu();
        int option = get_option();
        printf(YELLOW "---------------------------------\n" RESET);

        // Выбор опции программы
        switch (option)
        {
        // Выход из программы
        case 0:
            exit = true;
            break;
        // Ввод матрицы в виде матрицы
        case 1:
            status = get_matrix_coord(&matrix);
            break;
        // Ввод матрицы в виде матрицы из файла
        case 2:
            status = get_matr_file_matr(&matrix);
            break;
        // Ввод матрицы в координатном виде из файла
        case 3:
            status = get_matrix_coord_file(&matrix);
            break;
        // Генерация матрицы с заданными параметрами
        case 4:
            ;
            break;
        // Вывод матрицы в матричном виде
        case 5:
            if (matrix.nz_count > 0 && (matrix.cols > MAX_CONSOLE_SIZE || matrix.rows > MAX_CONSOLE_SIZE))
            {
                printf(RED "<!> Внимание, размерность матрицы не позволяет вывести её в виде матрицы в консоль\n" RESET);
            }
            else
                sp_print(stdout, &matrix);
            break;
        // Вывод матрицы в координатном виде
        case 6:
            printf(CYAN "Формат вывода:\n"
            YELLOW
            "Первый столбец - i, второй - j, третий - значение элемента\n"
            RESET);
            sp_print_coord(stdout, &matrix);
            break;
        // Вывод матрицы в виде трёх объектов
        case 7:
            printf(CYAN "Формат вывода:\n"
            YELLOW 
            "A - массив ненулевых элементов\n" 
            "JA - массив индексов столбцов элементов\n" 
            "IA - массив номеров компонент, с которых начинается описание строки\n" 
            RESET);
            sp_print_raw(stdout, &matrix);
            break;
        // Ввести вектор для умножения в координатном виде
        case 8:
            status = get_matrix_coord_vec(&vector);
            break;
        // Ввести вектор для умножения в координатном виде
        case 9:
            status = get_matrix_coord_file_vec(&vector);
            break;
        // Ввести вектор для умножения в виде массива
        case 10:
            status = get_matr_file_matr_vec(&vector);
            break;
        // Умножение стандартным методом
        case 11:
            status = standart_mult(&matrix, &vector);
            break;
        // Умножение быстрым методом
        case 12:
            status = fast_mult(&matrix, &vector);
            break;
        // Сравнение
        case 13:
            status = mult_diff(&matrix, &vector);
            break;
        // Вывести вектор на экран
        case 14:
            status = print_vector_to_console(&vector);
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }

        if (status != SUCCESS && (status != BAD_FILE || status != FAILURE))
        {
            status = SUCCESS;
            printf(RED "<!> Ошибка ввода.\n\n" RESET);
        }
        // Проверка на корректное завершение программы
        if (status != SUCCESS)
            exit = true;
    }
    return status;
}