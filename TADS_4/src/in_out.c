#include "in_out.h"

void console_input();
void print_greeting_message();

// Ввод для стека-списка
void push_list_stack_input(list_stack_head *list_st)
{
    err_stat error_control = no_err;
    int count = 0;
    char buff[MAX_LIST_STACK_ELEMS * 2 - 1];
    
    printf(YELLOW "Введите символы, которые вы хотите добавить в стек: " RESET);
    // Освобождение потока ввода
    int ch;
    while((ch = getchar()) != EOF && ch != '\n');
    if (fgets(buff, sizeof(buff), stdin) != buff)
        printf(RED "Ошибка: " RESET "Строка была введена неверно.\n");
    else
    {
        for (int i = 0; i < strlen(buff) && error_control == no_err; i++)
        {
            if (buff[i] == '\r' || buff[i] == '\n' || buff[i] == '\0')
                ;
            else if (i % 2 != 0 && buff[i] != ' ')
                error_control = err_input_error;
            else if (i % 2 == 0)
                error_control = stack_push_list(list_st, *(buff + i));
        }   
    }
    if (error_control == err_list_stack_overflow)
    {
        printf(RED "Ошибка: " RESET "Возникла ошибка переполнения стека.\n");
        stack_delete_list(list_st);
    }
    if (error_control != no_err)
    {
        printf(RED "Ошибка: " RESET "Возникла ошибка при добавлении элементов в стек.\n");
        stack_delete_list(list_st);
    }
}

// Ввод для удаления элементов
void pop_list_stack_input(list_stack_head *list_st)
{
    err_stat result = no_err;
    char elem;
    result = stack_pop_list(list_st, &elem);
    if (result == err_holow_stack_exclude_list)
        printf(RED "Ошибка: " RESET "Возникла ошибка при попытке удалить элемент из пустого стека.\n");
    if (result == no_err)
        printf(YELLOW "Извлечённый элемент: " RESET "%c\n", elem);
}

// Очистка стека
void clear_list_stack_input(list_stack_head *list_st)
{
    stack_delete_list(list_st);
}

// Обработка данных соотвественно заданию
void process_list_stack_input(list_stack_head *list_st)
{
    process_list_str();
}

// Вывод содержимого списка на экран
void show_list_stack_input(list_stack_head *list_st)
{
    err_stat error_control = no_err;
    if (list_st->elem_col == 0)
        printf("Список пуст.\n");
    else
    {
        printf(YELLOW "Все элементы стека: " RESET);
        stack_print_list(*list_st);
        printf("\n");
    }
}

// Ввод стека-массива
void push_array_stack_input(array_stack *arr_st)
{
    err_stat error_control = no_err;
    int count = 0;
    char buff[MAX_LIST_STACK_ELEMS * 2 - 1];

    if (arr_st->begin == NULL)
    {
        printf(YELLOW "Введите количество элементов, под которое следует выделить память: " RESET);
        int ch;
        while((ch = getchar()) != EOF && ch != '\n');
        if (scanf("%i", &ch) != 1 || ch < 1)
            printf(RED "Ошибка: " RESET "Введённые данные некорректны.\n");
        else if (ch > MAX_LIST_STACK_ELEMS * 2 - 1)
            printf(RED "Ошибка: " RESET "Попытка превысить ограничение по памяти.\n");
        else
        {
            if (stack_create_arr(ch, arr_st) != no_err)
                printf(RED "Ошибка: " RESET "При выделении памяти под стек произошла ошибка.\n");
            else
            {
                printf(YELLOW "Память выделена успешно\n" RESET);
            }
        }
    }

    if (arr_st->begin != NULL)
    {
        printf(YELLOW "Введите символы, которые вы хотите добавить в стек: " RESET);
        // Освобождение потока ввода
        int ch;
        while((ch = getchar()) != EOF && ch != '\n');
        if (fgets(buff, sizeof(buff), stdin) != buff)
            printf(RED "Ошибка: " RESET "Строка была введена неверно.\n");
        else
        {
            for (int i = 0; i < strlen(buff) && error_control == no_err; i++)
            {
                if (buff[i] == '\r' || buff[i] == '\n' || buff[i] == '\0')
                    ;
                else if (i % 2 != 0 && buff[i] != ' ')
                    error_control = err_input_error;
                else if (i % 2 == 0)
                    error_control = stack_push_arr(arr_st, *(buff + i));
            }   
        }
        if (error_control == err_stack_overflow_arr)
        {
            printf(RED "Ошибка: " RESET "Возникла ошибка переполнения стека.\n");
            stack_delete_arr(arr_st);
        }
        else if (error_control != no_err)
        {
            printf(RED "Ошибка: " RESET "Возникла ошибка при добавлении элементов в стек.\n");
            stack_delete_arr(arr_st);
        }
    }
}

// Удаление элементов списка-массива
void pop_array_stack_input(array_stack *arr_st)
{
    err_stat result = no_err;
    char elem;
    result = stack_pop_arr(arr_st, &elem);
    if (result == err_holow_stack_exclude_arr)
        printf(RED "Ошибка: " RESET "Попытка удалить элемент из пустого стека.\n");
    if (result == no_err)
    {
        printf(YELLOW "Извлечённый элемент: " RESET "%c\n", elem);
        printf(YELLOW "Освобождён адрес: " GREEN" %ld\n" RESET, arr_st->pointer - 1);
    }
}

// Очистка стека-массива
void clear_array_stack_input(array_stack *arr_st)
{
    stack_delete_arr(arr_st);
}

// Обработка данных соответственно заданию
void process_array_stack_input(array_stack *arr_st)
{
    process_arr_str();
}

// Вывод содержимого массива на экран
void show_array_stack_input(array_stack *arr_st)
{
    // Проверка на пустой стек
    if (arr_st->begin - 1 == arr_st->end)
        printf("Массив пуст.\n");
    else if (arr_st->begin == NULL)
        printf(RED "Ошибка: " RESET "Массив был очищен.\n");
    else
    {
        printf(YELLOW "Все элементы стека: " RESET);
        stack_print_arr(*arr_st);
        printf("\n");
    }
}

// Вывод вспомогательного сообщения
void print_help_message()
{
    printf(CYAN "Список возможных команд:\n" RESET
YELLOW "list" RESET " - переводит программу в режим работы\n\
со стеком-списком, для того, чтобы выйти из режима необходимо ввести команду exit\n"\
YELLOW "array" RESET " - переводит программу в режим работы\n\
со стеком-массивом, для того, чтобы выйти из режима необходимо ввести команду exit\n"\
YELLOW "push" RESET " - добавить элемент(ы) в стек, элементы вводятся через\n\
пробел, при наличии ошибки программа выведет сообщение об этом и\n\
выведет соотвествующее сообщение, стек при этом будет очищен.\n"
YELLOW "pop" RESET " - убирает элемент из стека\n\
при наличии ошибки программа выведет сообщение об этом и\n\
выведет соотвествующее сообщение, стек при этом будет очищен.\n"\
YELLOW "clear" RESET " - очищает текущий стек и выводит соответствующую \
информацию.\n"\
YELLOW "process" RESET " - переходит в режим обработки выражения\n"\
YELLOW "show" RESET " - выводит стек в консоль\n"\
YELLOW "help" RESET " - выводит все команды и их значение\n"\
YELLOW "exit" RESET " - выходит из текущего режима,  завершая\n\
работу со стеком. Информация о стеке при этом не выводится.\n");
}

// Вывод приветсвтенного сообщения
void print_greeting_message()
{
    printf(CYAN "Программа для работы со стеком\n" RESET);
    printf("Данная программа позволяет создавать стек двумя различными способами.\n");
    printf("Для работы в данной программе вам необходимо использовать команды,\n");
    printf("представленные ниже. Команды должны вводится также, как они записаны ниже\n");
    printf("Наличие лишних символов до или после команды будет расценено, как ошибка.\n");
    printf("В стеке списке количество элементов ограничено %i.\n", MAX_LIST_STACK_ELEMS);
    printf("при превышении этого количества программа закончит ввод и вернёт ошибку.\n");
    print_help_message();
}

// Обработка консольного ввода
void console_input()
{
    menu_status status = main_menu;
    list_stack_head list_st;
    array_stack arr_st = { NULL, NULL, NULL };

    char command[10];

    printf(CYAN "Главное меню.\n" RESET
    "Введите" CYAN " exit " RESET "для выхода из программы,\n"
    "введите" CYAN " list " RESET "для входа в режим работы со стеком-списком,\n"
    "введите" CYAN " array " RESET "для входа в режим работы со стеком-массивом.\n");

    while (status != exit_prog)
    {
        printf(YELLOW "Введите команду: " RESET);
        scanf("%s", command);
        // Команда вывода сообщения с инструкциями
        if (strcmp("help", command) == 0)
            print_help_message();
        // Переход в режим работы со стеком-списком
        else if (strcmp("list", command) == 0)
        {
            printf(CYAN "Вы перешли в режим работы со стеком-списком.\n" RESET);
            if (status != main_menu)
                printf(RED "Ошибка: " RESET "Вы не находитесь в главном меню.\n"
                "Введите команду " RESET CYAN "exit " RESET "чтобы выйти в главное меню и выбрать нужный вам режим.\n");
            else
            status = list_stack_menu;
        }
        // Переход в режим работы со стеком-массивом
        else if (strcmp("array", command) == 0)
        {
            if (status != main_menu)
                printf(RED "Ошибка:  " RESET "Вы не находитесь в главном меню.\n"
                "Введите команду " RESET CYAN "exit " RESET "чтобы выйти в главное меню и выбрать нужный вам режим.\n");
            else
            {
                printf(CYAN "Вы перешли в режим работы со стеком-массивом.\n" RESET);
                status = array_stack_menu;
            }
        }
        // Ввод элементов в стек
        else if (strcmp("push", command) == 0)
        {
            if (status != list_stack_menu && status != array_stack_menu)
                printf(RED "Ошибка: " RESET "Вы не находитесь в режиме работы со стеком.\n"
                "Введите команду " RESET CYAN "array " RESET "или " CYAN "list " RESET\
                "чтобы перейти в режим работы с нужным вам стеком.\n");
            else
            {
                if (status == list_stack_menu)
                    push_list_stack_input(&list_st);
                else if (status == array_stack_menu)
                    push_array_stack_input(&arr_st);
            }
        }
        // Удаление элементов из стека
        else if (strcmp("pop", command) == 0)
        {
            if (status != list_stack_menu && status != array_stack_menu)
                printf(RED "Ошибка: " RESET "Вы не находитесь в режиме работы со стеком.\n"
                "Введите команду " RESET CYAN "array " RESET "или " CYAN "list " RESET\
                "чтобы перейти в режим работы с нужным вам стеком.\n");
            else
            {
                if (status == list_stack_menu)
                    pop_list_stack_input(&list_st);
                else if (status == array_stack_menu)
                    pop_array_stack_input(&arr_st);
            }
        }
        // Очистка стека
        else if (strcmp("clear", command) == 0)
        {
            if (status != list_stack_menu && status != array_stack_menu)
                printf(RED "Ошибка: " RESET "Вы не находитесь в режиме работы со стеком.\n"
                "Введите команду " RESET CYAN "array " RESET "или " CYAN "list " RESET\
                "чтобы перейти в режим работы с нужным вам стеком.\n");
            else
            {
                if (status == list_stack_menu)
                    clear_list_stack_input(&list_st);
                else if (status == array_stack_menu)
                    clear_array_stack_input(&arr_st);
            }
        }
        // Выполение программы с помощью нужного стека
        else if (strcmp("process", command) == 0)
        {
            if (status != list_stack_menu && status != array_stack_menu)
                printf(RED "Ошибка: " RESET "Вы не находитесь в режиме работы со стеком.\n"
                "Введите команду " RESET CYAN "array " RESET "или " CYAN "list " RESET\
                "чтобы перейти в режим работы с нужным вам стеком.\n");
            else
            {
                if (status == list_stack_menu)
                    process_list_stack_input(&list_st);
                else if (status == array_stack_menu)
                    process_array_stack_input(&arr_st);
            }
        }
        // Вывод стека в консоль
        else if (strcmp("show", command) == 0)
        {
            if (status != list_stack_menu && status != array_stack_menu)
                printf(RED "Ошибка: " RESET "Вы не находитесь в режиме работы со стеком.\n"
                "Введите команду " RESET CYAN "array " RESET "или " CYAN "list " RESET\
                "чтобы перейти в режим работы с нужным вам стеком.\n");
            else
            {
                if (status == list_stack_menu)
                    show_list_stack_input(&list_st);
                else if (status == array_stack_menu)
                    show_array_stack_input(&arr_st);
            }
        }
        // Выход из меню и программ
        else if (strcmp("exit", command) == 0)
        {
            if (status == main_menu)
                status = exit_prog;
            else
            {
                if (status == list_stack_menu && list_st.head != NULL)
                    stack_delete_list(&list_st); 
                if (status == array_stack_menu && arr_st.begin != NULL)
                    stack_delete_arr(&arr_st);
                status = main_menu;
                printf(CYAN "Вы перешли в главное меню.\n" RESET
                "Введите" CYAN " exit " RESET "для выхода из программы,\n"
                "введите" CYAN " list " RESET "для входа в режим работы со стеком-списком,\n"
                "введите" CYAN " array " RESET "для входа в режим работы со стеком-массивом.\n");
            }
        }
        else
        {
            printf(RED "Ошибка: " RESET "Команда была введена неправильно.\n"
                "Введите команду " RESET CYAN "help " RESET\
                "чтобы вывести на экран список всех команд.\n");
        }
        printf("\n");
    }
}