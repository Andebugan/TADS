#ifndef _IN_OUT_H_
#define _IN_OUT_H_

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#include "head.h"
#include "stack_arr.h"
#include "stack_list.h"
#include "process.h"
#include <time.h>

typedef enum
{
    main_menu,
    list_stack_menu,
    array_stack_menu,
    exit_prog
} menu_status;

void console_input();
void print_greeting_message();

#endif