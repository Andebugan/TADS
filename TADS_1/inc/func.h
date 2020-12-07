#ifndef _FUNC_H_
#define _FUNC_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MANTISS_LEN 30
#define POWER_LEN 5
#define MAX_INPUT_LEN 256
#define MAX_POWER 99999

typedef enum
{
    execution_complete,
    err_sign_incorrect,
    err_power_sign_incorrect,
    err_mantiss_too_big,
    err_mantiss_incorrect,
    err_exponent_incorrect,
    err_power_incorrect,
    err_power_overflow,
    err_del_incorrect,
    err_second_num_zero,
} error_status;

typedef struct{
    char mantissa_sign;
    signed short int mantissa_arr[30];
    char power_sign;
    int power;
} large_num;

#endif