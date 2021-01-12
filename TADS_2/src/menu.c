#include <stdio.h>
#include <stdbool.h>
#include "status_codes.h"
#include "country_table.h"
#include "menu.h"

// === вспомогатеные функции === //

// показывает главное меню
static void show_main_menu(void);

// считывает введёную строку с номером опции
static int get_option(void);

// ============================= //

int run_main_menu(void)
{
    int status = SUCCESS;
    bool exit = false;

    while (!exit)
    {
        show_main_menu();
        int option = get_option();

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
