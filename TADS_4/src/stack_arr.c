#include "stack_arr.h"

err_stat stack_create_arr(int stack_len, array_stack *stack_arr);
void stack_delete_arr(array_stack *stack_arr);
err_stat stack_push_arr(array_stack *stack_arr, char elem);
err_stat stack_pop_arr(array_stack *stack_arr, char *elem);

/** 
 * Функция создания массива для реализации стека
 * После её выполнения указатель на начало массива указывает на выделеный стек, указатель стека имеет
 * значение, указывающее на элемент, находящийся перед первым элементом массива
 */
err_stat stack_create_arr(int stack_len, array_stack *stack_arr)
{
    err_stat result = no_err;
    // Инициализация указателя стека
    stack_arr->pointer = NULL;
    // Выделение памяти под stack_len элементов
    stack_arr->begin = malloc(stack_len * sizeof(char));
    // Проверка указателя на правильность выделения
    if (stack_arr->begin == NULL)
        result = err_malloc_arr;
    // Присвоение указателю стека значения элемента за началом стека и 
    if (result == no_err)
    {
        stack_arr->pointer = stack_arr->begin - 1;
        stack_arr->end = stack_arr->begin + (stack_len - 1);
    }
    return result;
}

// Функция удаления стека - освобождает память, занимаемую стеком
void stack_delete_arr(array_stack *arr_st)
{
    if (arr_st->begin != NULL)
    {
        while (arr_st->begin <= arr_st->end)
        {
            if (arr_st->end > arr_st->pointer)
                printf(YELLOW "Освобождён пустой адрес: " RED" %ld\n" RESET, arr_st->end);
            else
                printf(YELLOW "Освобождён занятый адрес: " GREEN" %ld\n" RESET, arr_st->end);
            arr_st->end--;
        }
        free(arr_st->begin);
        arr_st->begin = NULL;
        arr_st->end = NULL;
    }
    else
        printf(RED "Ошибка: " RESET "Массив уже очищен.\n");
}

/**
 * Функция добавления в стек нового элемента
 * Проверяет, находится ли указатель на последнем элементе, в этом случае возвращается ошибка
 * переполнения стека
 * Если ошибки нет, то указатель смещается на длину элемента вправо и по его адресу записывается
 * значение элемента elem
 */
err_stat stack_push_arr(array_stack *stack_arr, char elem)
{
    err_stat result = no_err;

    // Проверяем массив на переполнение
    if (result == no_err && stack_arr->end == stack_arr->pointer)
        result = err_stack_overflow_arr;
    // Если всё верно, то добавляем значение элемента в массив 
    if (result == no_err)
    {
        stack_arr->pointer++;
        *stack_arr->pointer = elem;
    }
    return result;
}

/**
 * Функция удаления из массива элементов
 * Если указатель стека указывает на элемент перед начальным, то возвращается ошибка
 * попытки извлечения из пустого стека
 * Иначе элемент извлекается и указатель стека смещается вправо
 */
err_stat stack_pop_arr(array_stack *stack_arr, char *elem)
{
    err_stat result = no_err;

    // Проверка на пустой стек
    if (stack_arr->begin - 1 == stack_arr->pointer)
        result = err_holow_stack_exclude_arr;
    // Проверка на пустой стек
    if (stack_arr->begin == NULL)
        result = err_holow_stack_exclude_arr;
    // Вывод элемента из стека
    if (result == no_err)
    {
        *elem = *stack_arr->pointer;
        stack_arr->pointer--;
    }
    return result;
}

// Функция вывода всех элементов на экран
err_stat stack_print_arr(array_stack stack_arr)
{
    err_stat result = no_err;
    
    // Вывод элемента из стека
    while (stack_arr.begin - 1 != stack_arr.pointer)
    {
        printf("%c ", *stack_arr.pointer);
        stack_arr.pointer--;
    }
    return result;
}