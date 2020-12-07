#include "stack_arr.h"
#include "stack_list.h"
#include "in_out.h"
#include "process.h"
#include <sys/time.h>

// Проверка расстановки скобок в стеке-массиве
void process_arr_str()
{
    array_stack brackets = { NULL, NULL, NULL };
    err_stat result = no_err;
    char str_in[3] = "({[";
    char str_check[3] = "]})";
    char buff = ' ';
    char str[MAX_LIST_STACK_ELEMS];
    struct timeval  tv1, tv2;
    //Ввод данных
    printf(YELLOW "Введите выражение: " RESET);
    // Очистка stdin
    int ch;
    while((ch = getchar()) != EOF && ch != '\n');
    if (fgets(str, MAX_LIST_STACK_ELEMS, stdin) != str)
    {
        printf(RED "Ошибка: " RESET "Строка введена неверно\n");
        result = err_input_error;
    }

    if (strlen(str) == 1)
    {
        printf(RED "Ошибка: " RESET "Введено пустое выражение\n");
        result = err_input_error;
    }
    // Создание стека-массива
    gettimeofday(&tv1, NULL);
    if (result == no_err)
        result = stack_create_arr(strlen(str) - 1, &brackets);
    if (result != no_err && result != err_input_error)
        printf(RED "Ошибка: " RESET "Возникла ошибка при попытке создать стек-массив.\n");
    for (int i = 0; i < strlen(str) && result == no_err; i++)
    {
        if (strchr(str_in, str[i]) != NULL)
            result = stack_push_arr(&brackets, str[i]);
        else if (strchr(str_check, str[i]) != NULL)
        {
            result = stack_pop_arr(&brackets, &buff);
            if (result == err_holow_stack_exclude_arr)
                result = -1;
            if (buff == '(' && str[i] != ')')
                result = -1;
            else if (buff == '[' && str[i] != ']')
                result = -1;
            else if (buff == '{' && str[i] != '}')
                result = -1;
        }
    }
    if (brackets.begin - 1 != brackets.pointer && result != err_input_error)
        result = -1;
    gettimeofday(&tv2, NULL);
    if (result == no_err)
    {
        printf(YELLOW "Расстановка скобок: " GREEN " Правильно.\n" RESET);
        printf(YELLOW "Памяти использовано: " CYAN " %i\n" RESET, (strlen(str) - 1)*sizeof(char));
        stack_delete_arr(&brackets);
        printf(YELLOW "Время выполнения - " CYAN "%lf" YELLOW " мкс.\n" RESET,\
        (double)(tv2.tv_sec - tv1.tv_sec) * 1000000 + (double)(tv2.tv_usec - tv1.tv_usec));  
    }
    else if (result == -1)
    {
        printf(YELLOW "Расстановка скобок: " RED " Неправильно.\n" RESET);
        printf(YELLOW "Памяти использовано: " CYAN " %i\n" RESET, (strlen(str) - 1)*sizeof(char));
        stack_delete_arr(&brackets);
        printf(YELLOW "Время выполнения - " CYAN "%lf" YELLOW " секунд.\n" RESET,\
        (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));
    }
    else if (result != err_input_error)
        printf(RED "Ошибка: " RESET "Возникла ошибка при обработке выражения.\n");  
}

// Проверка расстановки скобок в стеке-списке
void process_list_str()
{
    list_stack_head brackets = { 0, NULL };
    err_stat result = no_err;
    char str_in[3] = "({[";
    char str_check[3] = "]})";
    char buff = ' ';
    char str[MAX_LIST_STACK_ELEMS];
    struct timeval  tv1, tv2;
    //Ввод данных
    printf(YELLOW "Введите выражение: " RESET);
    // Очистка stdin
    int ch;
    while((ch = getchar()) != EOF && ch != '\n');
    if (fgets(str, MAX_LIST_STACK_ELEMS, stdin) != str)
    {
        printf(RED "Ошибка: " RESET "Строка введена неверно\n");
        result = err_input_error;
    }

    if (strlen(str) == 1)
    {
        printf(RED "Ошибка: " RESET "Введено пустое выражение\n");
        result = err_input_error;
    }
    // Алгоритм поиска скобок
    gettimeofday(&tv1, NULL);
    for (int i = 0; i < strlen(str) && result == no_err; i++)
    {
        if (strchr(str_in, str[i]) != NULL)
            result = stack_push_list(&brackets, str[i]);
        else if (strchr(str_check, str[i]) != NULL)
        {
            result = stack_pop_list(&brackets, &buff);
            if (result == err_holow_stack_exclude_arr)
                result = -1;
            if (buff == '(' && str[i] != ')')
                result = -1;
            else if (buff == '[' && str[i] != ']')
                result = -1;
            else if (buff == '{' && str[i] != '}')
                result = -1;
        }
    }
    if (brackets.elem_col > 0 && result != err_input_error)
        result = -1;
    gettimeofday(&tv2, NULL);
    if (result == no_err)
    {
        printf(YELLOW "Расстановка скобок: " GREEN " Правильно.\n" RESET);
        printf(YELLOW "Памяти использовано: " CYAN " %i\n" RESET, (strlen(str) - 1)*sizeof(char));
        stack_delete_list(&brackets);
        printf(YELLOW "Время выполнения - " CYAN "%lf" YELLOW " секунд.\n" RESET,\
        (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec)); 
    }
    else if (result == -1)
    {
        printf(YELLOW "Расстановка скобок: " RED " Неправильно.\n" RESET);
        printf(YELLOW "Памяти использовано: " CYAN " %i\n" RESET, (strlen(str) - 1)*sizeof(char));
        stack_delete_list(&brackets);
        printf(YELLOW "Время выполнения - " CYAN "%lf" YELLOW " секунд.\n" RESET,\
        (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));
    }
    else if (result != err_input_error)
        printf(RED "Ошибка: " RESET "Возникла ошибка при обработке выражения.\n");  
}