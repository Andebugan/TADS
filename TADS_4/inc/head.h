#ifndef _HEAD_H_
#define _HEAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum
{
    no_err,
    err_stack_overflow_arr,
    err_holow_stack_exclude_arr,
    err_holow_stack_exclude_list,
    err_malloc_arr,
    err_elem_pointer_null,
    err_list_stack_overflow,
    err_malloc_list,
    err_wrong_status,
    err_input_error
} err_stat;

#endif