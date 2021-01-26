#include <stdio.h>
#include <stdbool.h>
#include "status_codes.h"
#include "country_table.h"
#include "country_sort.h"
#include "menu.h"
#include "colors.h"

// Макросы для управления вводом/выводом
#define SAFE_SCANF(func)   \
    if (status == SUCCESS) \
    status = func

#define SAFE_PRINTF(func)  \
    if (status == SUCCESS) \
    func

// Вывод главного меню
static void show_main_menu(void);

// Считывание введённой строки с номером опции
static int get_option(void);

// Обработка статуса программы
static int handle_status(int status);

// Добавление записи с клавиатуры
static int __add_country(void);

// Быстрая сортировка с выводом времени
void cntr_fast_table(void)
{
    unsigned long long time = 0;
    time = ctrt_sort_fast_table();
    printf(CYAN "Таблица была отсортирована быстрым способом с помощью основной таблицы\n"
    "Время сортировки: " RESET "%llu\n", time);
}

// Медленная сортировка с выводом времени
void cntr_slow_table(void)
{
    unsigned long long time = 0;
    time = ctrt_sort_slow_table();
    printf(CYAN "Таблица была отсортирована медленным способом с помощью основной таблицы\n"
    "Время сортировки: " RESET "%llu\n", time);
}

// Быстрая сортировка ключей с выводом времени
void cntr_fast_keys(void)
{
    unsigned long long time = 0;
    time = ctrt_sort_fast_keys();
    printf(CYAN "Таблица ключей была отсортирована быстрым способом\n"
    "Время сортировки: " RESET "%llu\n", time);
}

// Медленная сортировка ключей с выводом времени
void cntr_slow_keys(void)
{
    unsigned long long time = 0;
    time = ctrt_sort_slow_keys();
    printf(CYAN "Таблица ключей была отсортирована медленным способом\n"
    "Время сортировки: " RESET "%llu\n", time);
}

// Поиск по вариантному полю на указанном континенте указанного вида спорта
int ctrt_search(void)
{
    int status = SUCCESS;
    continent_t continent;
    sport_t sport;
    status = ctr_search(&continent, &sport);
    if (status == SUCCESS)
        ctrt_select(continent, sport);
    return status;
}

void count_effectivness(void)
{
    float mtime_t_fast = 0;
    float mtime_t_slow = 0;
    float mtime_k_fast = 0;
    float mtime_k_slow = 0;

    mtime_t_fast = ctrt_mid_time(country_table_size, ctrt_sort_fast_table, ctrt_shuffle_table);
    mtime_t_slow = ctrt_mid_time(country_table_size, ctrt_sort_slow_table, ctrt_shuffle_table);
    mtime_k_fast = ctrt_mid_time(country_table_size, ctrt_sort_fast_keys, ctrt_shuffle_key);
    mtime_k_slow = ctrt_mid_time(country_table_size, ctrt_sort_fast_keys, ctrt_shuffle_key);

    // Ключи относительно таблицы - какой объём памяти/времени используется при сортировке ключами относительно таблицы
    printf("Эффективность быстрой сортировки по времени (ключи относительно таблицы): %.2f%%\n", mtime_k_fast/mtime_t_fast * 100);
    printf("Эффективность медленной сортировки по времени (ключи относительно таблицы): %.2f%%\n", mtime_k_slow/mtime_t_slow * 100);
    printf("Эффективность сортировки ключей по времени (быстрая относительно медленной): %.2f%%\n", mtime_k_fast/mtime_k_slow * 100);
    printf("Эффективность сортировки таблицы по времени (быстрая относительно медленной): %.2f%%\n", mtime_t_fast/mtime_t_slow * 100);
    printf("Эффективность сортировки по памяти (таблица относительно ключей): %.2f%%\n",\
    (float)sizeof(country_t) / ((float)sizeof(tkey_t) + (float)sizeof(country_t))  * 100);
}

// Функция главного меню
int run_main_menu(void)
{
    // Инициализация кода выхода и переменной-флага выхода
    int status = SUCCESS;
    bool exit = false;

    // Цикл меню
    while (!exit)
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
        // Вывод таблицы на экран
        case 1:
            ctrt_print();
            break;
        // Вывод таблицы по ключам
        case 2:
            ctrt_print_by_keys();
            break;
        // Добавление записи с клавиатуры
        case 3:
            status = __add_country();
            status = handle_status(status);
            break;
        // Разупорядочивание таблицы
        case 4:
            ctrt_shuffle_table();
            printf(RED "Внимание, таблица ключей более не является отсортированной!\n" RESET);  
            break;
        // Разупорядочивание таблицы ключей
        case 5:
            ctrt_shuffle_key();
            break;
        // Быстрая сортировка основной таблицы
        case 6:
            cntr_fast_table();
            break;
        // Медленная сортировка основной таблицы
        case 7:
            cntr_slow_table();
            break;
        // Быстрая сортировка таблицы ключей
        case 8:
            cntr_fast_keys();
            break;
        // Медленная сортировка таблицы ключей
        case 9:
            cntr_slow_keys();
            break;
        // Вывод таблицы ключей на экран
        case 10:
            ctrt_print_keys();
            break;
        // Поиск по вариантному полю
        case 11:
            status = ctrt_search();
            break;
        // Удаление элемента из таблицы
        case 12:
            status = delete_by_index();
            break;
        // Сравнение эффективности алгоритма
        case 13:
            count_effectivness();
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }
        if (status != SUCCESS && (status != INVALID_FILENAME || status != MAX_COUNTRY_TABLE_SIZE_REACHED || status != FAILURE))
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

// Вывод главного меню
static void show_main_menu(void)
{
    printf(CYAN "МЕНЮ:\n" RESET
        YELLOW
        "  1. Вывести таблицу на экран\n"
        "  2. Вывести таблицу по ключам\n"
        "  3. Добавить запись вручную\n"
        "  4. Перемешать таблицу стран\n"
        "  5. Перемешать таблицу ключей\n"
        "  6. Быстрая сортировка основной таблицы\n"
        "  7. Медленная сортировка основной таблицы\n"
        "  8. Быстрая сортировка таблицы ключей\n"
        "  9. Медленная сортировка таблицы ключей\n"
        "  10. Вывод таблицы ключей на экран\n"
        "  11. Поиск по вариантному полю\n"
        "  12. Удаление элемента из таблицы по номеру\n"
        "  13. Сравнение эффективности алгоритмов\n"
        "  0. Выйти\n" RESET
        CYAN "> " RESET);
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

// Обработка статуса программы
static int handle_status(int status)
{
    switch (status)
    {
    case FAILURE:
        printf(RED "<!> Неопознанная ошибка.\n" RESET);
        break;
    
    case MAX_COUNTRY_TABLE_SIZE_REACHED:
        printf(RED "<!> Было достигнуто максимальное число записей в таблице.\n" RESET);
        break;

    default:
        return status;
    }

    return SUCCESS;
}

// Добавление записи, введённой с клавиатуры в таблицу
static int __add_country(void)
{
    // Инициализация статуса программы
    int status = MAX_COUNTRY_TABLE_SIZE_REACHED;
    // Создание переменной - элемента таблицы
    country_t country;

    // Проверка на удовлетворение условию о количестве записей
    if (country_table_size < MAX_TABLE_SIZE)
    {
        // Считывание данных с клавиатуры
        status = ctr_prompt(&country);

        if (status == SUCCESS)
        {
            // Занесение данных в таблицу
            country_table[country_table_size] = country;
            key_table[country_table_size].index = country_table_size;
            key_table[country_table_size].population = country.population;
            country_table_size++;
        }
        printf(GREEN "Запись успешно добавлена в конец таблицы\n" RESET);
    }

    return status;
}
