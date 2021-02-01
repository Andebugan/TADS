#include "menu.h"

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
        "  1. Симулировать прогон 1000 заявок\n"
        "  2. Изменить время обработки заявок в ОА\n"
        "  0. Выйти\n" RESET
        CYAN "> " RESET);
}

// Вывести параметры СМА
void print_worker_stats(void)
{
    printf(CYAN "===== Параметры СМА: =====\n" RESET);
    printf(CYAN "  Очередь 1:\n" RESET);
    printf(YELLOW"    Длина очереди: " RESET "%u\n", worker_stats.qu_1.curr_length);
    printf(YELLOW"    Средняя длина: " RESET "%.1f\n", worker_stats.qu_1.mean_length);
    printf(YELLOW"    Среднее время: " RESET "%.1f\n", worker_stats.qu_1.mean_time);
    printf(CYAN "  Очередь 2:\n" RESET);
    printf(YELLOW"    Длина очереди: " RESET "%u\n", worker_stats.qu_2.curr_length);
    printf(YELLOW"    Средняя длина: " RESET "%.1f\n", worker_stats.qu_2.mean_length);
    printf(YELLOW"    Среднее время: " RESET "%.1f\n", worker_stats.qu_2.mean_time);
    printf(YELLOW"  Время моделирования: " RESET "%.2f\n", worker_stats.model_time);
    printf(YELLOW"  Время простоя второго ОА:" RESET " %.2f\n", worker_stats.free_time);
    printf(YELLOW"  Кол-во срабатываний первого " RESET "ОА: %u\n", worker_stats.activations);
}

// Меню симуляции очереди
void show_process_menu(void)
{
    printf(CYAN "Симуляция очереди:\n" RESET
        YELLOW
        "  Опции ввода:\n"
        RESET
        "  1. Симулировать прогон 1000 заявок для очереди-списка\n"
        "  2. Симулировать прогон 1000 заявок для очереди-массива\n"
        "  3. Сравнить результаты прогона для очереди-массива и очереди-списка\n"
        "  4. Включить/выключить режим демонстрации хранения памяти для проверки на дефрагментацию\n"
        "  0. Выйти\n" RESET
        CYAN "> " RESET);
}

// Обработка заявок у очереди-списка
void process_SMA_list(void)
{
    worker_use(WORKER_LIST, DEFAULT_QUEUE_CAPACITY);

    print_worker_stats();

    for (int i = 0; i < 10; i++)
    {
        worker_run(100);
        print_worker_stats();
    }

    worker_destroy();
}

// Обработка заявок у очереди-массива
void process_SMA_arr(void)
{
    worker_use(WORKER_ARRAY, DEFAULT_QUEUE_CAPACITY);

    print_worker_stats();
    
    for (int i = 0; i < 10; i++)
    {
        worker_run(100);
        print_worker_stats();
    }

    worker_destroy();
}

// Сравнение результатов прогона очереди-массива и очереди-списка
void res_SMA_diff(void)
{
    ;
}

// Проверка памяти
void set_mem_check(void)
{
    ;
}

// Обработка заявок
void process_applications(void)
{
    bool exit = false;
    while (!exit)
    {
        show_process_menu();
        int option = get_option();
        // Выбор опции программы
        switch (option)
        {
        // Выход из программы
        case 0:
            exit = true;
            break;
        // Прогон для 1000 заявок очереди-списка
        case 1:
            process_SMA_list();
            break;
        // Прогон для 1000 заявок очереди-массива
        case 2:
            process_SMA_arr();
            break;
        // Сравнение результатов
        case 3:
            res_SMA_diff();
            break;
        // Режим демонстрации памяти
        case 4:
            set_mem_check();
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }
    }
}

// Меню изменения параметров ОА
void show_OA_menu(void)
{
    printf(CYAN "Изменение параметров ОА:\n" RESET
        YELLOW
        "  Опции ввода:\n"
        RESET
        "  1. Изменить время обработки ОА1\n"
        "  2. Изменить время обработки ОА2\n"
        "  0. Выйти\n" RESET
        CYAN "> " RESET);
}

// Новое время обработки
void get_new_time(time_intv_t *time)
{
    float t_start = 0.0f;
    float t_end = 0.0f;
    printf(YELLOW
    "Введите новое время в формате [начало] [конец]\n"
    "числа должны соответствовать типу данных float:\n"
    RESET);
    if (fscanf(stdin, "%f %f", &t_start, &t_end) != 2\
    || t_start < -EPSILON || t_end < -EPSILON || t_start > t_end)
        printf(RED "<!> Ошибка, новое время обработки введено некорректно\n" RESET);
    else
    {
        time->max = t_end;
        time->min = t_start;
        printf(GREEN "Время обработки установлено успешно\n" RESET);
    }
    clean_input_stream();
}

// Изменение параметров ОА
void change_OA_params(void)
{
    bool exit = false;
    while (!exit)
    {
        show_OA_menu();
        int option = get_option();
        // Выбор опции программы
        switch (option)
        {
        // Выход из программы
        case 0:
            exit = true;
            break;
        // Изменение времени для OA1
        case 1:
            get_new_time(&T1);
            break;
        // Изменение времени для ОА2
        case 2:
            get_new_time(&T2);
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }
    }
}

// Функция работы с программой
int menu_loop(void)
{
    int status = SUCCESS;
    bool exit = false;
    T1 = (time_intv_t){ .min = 0.0f, .max = 6.0f };
    T2 = (time_intv_t){ .min = 1.0f, .max = 8.0f };
    probability = 0.7f;
    printf(YELLOW "Лабораторная работа №5\n" RESET);
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
        // Обработка заявок
        case 1:
            process_applications();
            break;
        // Изменение параметров ОА
        case 2:
            change_OA_params();
            break;
        // Ошибка ввода опции
        default:
            printf(RED "<!> Неверная опция.\n\n" RESET);
            break;
        }

        /*
        // Проверка на корректное завершение программы
        if (status != SUCCESS)
            exit = true;
        */
    }
    return status;
}