#include <stdio.h>
#include "status_codes.h"
#include "country_table.h"
#include "menu.h"
#include "colors.h"

// Главная функция программы
int main(void)
{
    int status = SUCCESS;

    // Создание таблицы из файла с данными
    status = ctrt_load_from_file("data.txt");
    if (status == SUCCESS)
        // Запуск цикла главного меню
        status = run_main_menu();

    // В случае ошибки:
    if (status != SUCCESS)
        printf(RED "status was not ok: %d\n" RESET, status);

    return status;
}
