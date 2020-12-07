#ifndef _STACK_ARR_H_
#define _STACK_ARR_H_

#include "head.h"
#include "in_out.h"

typedef struct
{
    char *begin;
    char *end;
    char *pointer;
} array_stack;

err_stat stack_create_arr(int stack_len, array_stack *stack_arr);
void stack_delete_arr(array_stack *stack_arr);
err_stat stack_push_arr(array_stack *stack_arr, char elem);
err_stat stack_pop_arr(array_stack *stack_arr, char *elem);
err_stat stack_print_arr(array_stack stack_arr);

#endif