#include "stack_list.h"

void stack_delete_list(list_stack_head *stack_list);
err_stat stack_push_list(list_stack_head *stack_list, char elem);
err_stat stack_pop_list(list_stack_head *stack_list, char *elem);

/**
 * Функция создания стека в представлении списка
 * Возвращает
 */
err_stat stack_push_list(list_stack_head *stack_list, char elem)
{
    err_stat result = no_err;
    list_stack *new_node;
    // Проверка на переполнение
    if (stack_list->elem_col == MAX_LIST_STACK_ELEMS)
        result = err_list_stack_overflow;
    // Выделение памяти под новый элемент
    if (result == no_err)
        new_node = (list_stack*)malloc(sizeof(list_stack));
    if (new_node == NULL)
        result = err_malloc_list;
    // Добавление нового элемента в список
    if (result == no_err)
    {
        new_node->data = elem;
        new_node->next = stack_list->head;
        stack_list->head = new_node;
        stack_list->elem_col++;
    }
    return result;
}

/**
 * Функция возвращения элемента из стека
 * Если производится попытка очистить пустой стек, то возвращается
 * ошибка
 * Иначе возвращается значение освобождённого элемента
 */
err_stat stack_pop_list(list_stack_head *stack_list, char *elem)
{
    err_stat result = no_err;
    list_stack *old_node = NULL;

    if (stack_list->elem_col == 0)
        result = err_holow_stack_exclude_list;
    if (result == no_err)
    {
        old_node = stack_list->head;
        *elem = stack_list->head->data;
        stack_list->head = stack_list->head->next;
        free(old_node);
        stack_list->elem_col--;
    }
    return result;
}

/**
 * Функция освобождения всего стека
 */
void stack_delete_list(list_stack_head *stack_list)
{
    list_stack *old_node = NULL;

    while (stack_list->elem_col > 0)
    {
        old_node = stack_list->head;
        stack_list->head = stack_list->head->next;
        free(old_node);
        stack_list->elem_col--;
    }
}

/**
 * Вывод содержимого списка на экран
 */
err_stat stack_print_list(list_stack_head stack_list)
{
    err_stat result = no_err;
    list_stack *old_node = NULL;
    char elem;
    while (stack_list.elem_col > 0)
    {
        if (stack_list.elem_col == 0)
        result = err_holow_stack_exclude_list;
        if (result == no_err)
        {
            old_node = stack_list.head;
            elem = stack_list.head->data;
            stack_list.head = stack_list.head->next;
            stack_list.elem_col--;
        }
        printf("%c ", elem);
    }
    return result;
}