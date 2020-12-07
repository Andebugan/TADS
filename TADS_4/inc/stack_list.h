#ifndef _STACK_LIST_H_
#define _STACK_LIST_H_

#define MAX_LIST_STACK_ELEMS 30

#include "head.h"

typedef struct list_stack
{
    struct list_stack *next;
    char data;
} list_stack;

typedef struct list_stack_head
{
    int elem_col;
    struct list_stack *head;
} list_stack_head;

void stack_delete_list(list_stack_head *stack_list);
err_stat stack_push_list(list_stack_head *stack_list, char elem);
err_stat stack_pop_list(list_stack_head *stack_list, char *elem);
err_stat stack_print_list(list_stack_head stack_list);

#endif