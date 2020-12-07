#ifndef _NUM_FUNCS_H_
#define _NUM_FUNCS_H_
#include "func.h"

int compare_nums(large_num num_1, large_num num_2);
large_num multiply_simple(large_num num, short int small_num);
large_num minus_nums(large_num num_1, large_num num_2);
error_status divide_nums(large_num first_num, large_num second_num, large_num *division_res);
int seek_del(large_num first_num, large_num second_num);
large_num move_mantiss_right(large_num result);
large_num move_mantiss_left(large_num result);
int check_del(large_num first_num, large_num second_num, large_num division_res);

#endif