#include <stdio.h>
#include <stdbool.h>
#include "status_codes.h"
#include "country_table.h"
#include "country_sort.h"
#include "menu.h"

// === вспомогатеные функции === //

// показывает главное меню
static void show_main_menu(void);

// считывает введёную строку с номером опции
static int get_option(void);

// обрабатывает коды ошибок и выводит соотв. сообщения
static int handle_status(int status);

// добавляет запись вводимую с клавиатуры
static int __add_country(void);

// ============================= //

int run_main_menu(void)
{
    int status = SUCCESS;
    bool exit = false;

    while (!exit)
    {
        printf("---------------------------------\n");
        show_main_menu();
        int option = get_option();
        printf("---------------------------------\n");

        switch (option)
        {
        case 0:
            exit = true;
            break;

        case 1:
            ctrt_print();
            break;

        case 2:
            ctrt_print_by_keys();
            break;

        case 3:
            status = __add_country();
            status = handle_status(status);
            break;

        case 4:
            ctrt_shuffle_table();
            break;
        
        case 5:
            ctrt_shuffle_key();
            break;

        default:
            printf("неверная опция.\n\n");
            break;
        }

        if (status != SUCCESS)
            exit = true;
    }

    return status;
}

// ============================= //

static void show_main_menu(void)
{
    printf("МЕНЮ:\n"
        "  1. Вывести список стран на экран\n"
        "  2. Вывести список стран по ключам\n"
        "  3. Добавить запись вручную\n"
        "  4. Перемешать таблицу стран\n"
        "  5. Перемешать таблицу ключей\n"
        "  0. Выйти\n"
        "> ");
}

// считывает введёную строку с номером опции
static int get_option(void)
{
    const int line_size = 16;
    char line[line_size];
    int option = -1;

    fgets(line, line_size, stdin);
    sscanf(line, "%d", &option);

    return option;
}

static int handle_status(int status)
{
    switch (status)
    {
    case FAILURE:
        printf("<!> Неопознанная ошибка.\n");
        break;
    
    case MAX_COUNTRY_TABLE_SIZE_REACHED:
        printf("<!> Было достигнуто максимальное число записей в таблице.\n");
        break;

    default:
        return status;
    }

    return SUCCESS;
}

static int __add_country(void)
{
    int status = MAX_COUNTRY_TABLE_SIZE_REACHED;
    country_t country;

    if (country_table_size < MAX_TABLE_SIZE)
    {
        status = ctr_prompt(&country);

        if (status == SUCCESS)
        {
            country_table[country_table_size] = country;
            key_table[country_table_size].index = country_table_size;
            key_table[country_table_size].population = country.population;
            country_table_size++;
        }
    }

    return status;
}
