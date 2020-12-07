#include "func.h"
#include "in_out.h"
//WORKING DEBBUGED
/**
 * Функция сравнения двух больших чисел, возвращает 1, если первое больше второго
 * и возвращает -1, если первое меньше, и 0 они равны
 */
int compare_nums(large_num num_1, large_num num_2)
{
    for (int i = 0; i < MANTISS_LEN; i++)
    {
        if (num_1.mantissa_arr[i] > num_2.mantissa_arr[i])
            return 1;
        else if (num_1.mantissa_arr[i] < num_2.mantissa_arr[i])
            return -1;
    }
    return 0;
}

//WORKING DEBBUGED
// Сдвинуть мантиссу вправо
large_num move_mantiss_right(large_num result)
{
    int round_num = 0;

    if (result.mantissa_arr[MANTISS_LEN - 1] != 0)
    {
        if (result.mantissa_arr[MANTISS_LEN - 1] > 5)
            round_num = 1;
    }

    for (int i = MANTISS_LEN - 1; i >= 0; i--)
    {
        result.mantissa_arr[i] = result.mantissa_arr[i - 1];
        result.mantissa_arr[i] += round_num;
        if (result.mantissa_arr[i] == 10)
        {
            result.mantissa_arr[i] = 0;
            round_num = 1;
        }
        else
            round_num = 0;
    }
    result.mantissa_arr[0] = 0;
    result.power += 1;
    return result;
}

// Округление числа
large_num mantiss_round(large_num result, int last_num)
{
    int round_num = 0;
    if (last_num > 4)
    {
        round_num = 1;
        for (int i = MANTISS_LEN - 1; i >= 0; i--)
        {
            result.mantissa_arr[i] += round_num;
            if (result.mantissa_arr[i] == 10)
            {
                result.mantissa_arr[i] = 0;
                round_num = 1;
            }
            else
                round_num = 0;
        }
    }
    return result;
}

// Сдвинуть мантиссу влево
large_num move_mantiss_left(large_num result)
{
    int round_num = 0;
    for (int i = 0; i < MANTISS_LEN - 1; i++)
    {
        result.mantissa_arr[i] = result.mantissa_arr[i + 1];
    }
    result.mantissa_arr[MANTISS_LEN - 1] = 0;
    result.power -= 1;
    return result;
}

//WORKING DEBBUGED
/**
 * Эта функция умножает большое число на однозначное число, если возникает переполнение, то 
 * функция возвращает число, полученное на входе, иначе, возвращает результат
 */
large_num multiply_simple(large_num num, int small_num)
{
    large_num result;
    int next_num = 0;
    int round_num = 0;
    int mult = 0;
    result = num;
    // Умножение на число
    for (int i = MANTISS_LEN - 1; i >= 0; i--)
    {
        mult = num.mantissa_arr[i] * small_num + next_num;
        if (mult < 10)
        {
                result.mantissa_arr[i] = mult;
                next_num = 0;
        }
        else
        {
            result.mantissa_arr[i] = mult % 10;
            next_num = mult / 10;
        }
    }

    // Добавление числа в начало мантиссы
    if (next_num != 0)
    {
        return num;
    }
    return result;
}

typedef enum
{
    normal,
    taken
} minus_state;
//WORKING DEBUGGED
/**
 * Функция вычитания двух больших чисел, первое число обязательно больше второго
 */
large_num minus_nums(large_num num_1, large_num num_2)
{
    // Замечание, когда 1234 - 1245 будет 0011!
    large_num res;
    minus_state status = normal;
    for (int i = MANTISS_LEN - 1; i >= 0; i--)
    {
        if (num_1.mantissa_arr[i] < num_2.mantissa_arr[i] && status == normal)
        {
            num_1.mantissa_arr[i] += 10;
            status = taken;
        }
        else if (num_1.mantissa_arr[i] <= num_2.mantissa_arr[i] && status == taken)
            num_1.mantissa_arr[i] += 9;
        else if (num_1.mantissa_arr[i] > num_2.mantissa_arr[i] && status == taken)
        {
            num_1.mantissa_arr[i] -= 1;
            status = normal;
        }
        res.mantissa_arr[i] = num_1.mantissa_arr[i] - num_2.mantissa_arr[i];
    }
    return res;
}

//WORKING
/**
 * Функция прохождения графа множителя, первое обязательно больше второго
 */
int seek_del(large_num first_num, large_num second_num)
{
    int del = 1;
    int compare_check_up = -1;
    int compare_check_down = -1;
    large_num up_num;
    large_num down_num;

    while (del < 10)
    {
        down_num = multiply_simple(second_num, del);
        up_num = multiply_simple(second_num, del + 1);
        if (compare_nums(up_num, down_num) == 0)
            return del - 1;
        if (compare_nums(up_num, second_num) == 0)
            return del;
        compare_check_down = compare_nums(first_num, down_num);
        compare_check_up = compare_nums(first_num, up_num);
        if (compare_nums(first_num, down_num) == 0)
            return del;
        else if (compare_nums(first_num, down_num) == 1 && compare_nums(first_num, up_num) == -1 )
            return del;
        del++;
    }
    return -1;
}

int check_null(large_num first_num)
{
    for (int i = 0; i < MANTISS_LEN; i++)
        if (first_num.mantissa_arr[i] != 0)
            return 1;
    return 0;
}

/**
 * Проверка на деление, возвращает 0, если не поделено, и 1 если поделено
 */
int check_del(large_num first_num, large_num second_num, large_num division_res)
{
    if (division_res.mantissa_arr[MANTISS_LEN - 1] != 0)
        return 0;

    for (int i = 0; i < MANTISS_LEN; i++)
        if (first_num.mantissa_arr[i] != 0)
            return 1;
    
    return 0;
}

/**
 * Функция деления числа на число, первое делится на второе
 */
error_status divide_nums(large_num first_num, large_num second_num, large_num *division_res)
{
    error_status result = execution_complete;
    int del = 0;
    int i_div = 0;

    if (first_num.mantissa_sign == '+' && second_num.mantissa_sign == '-')
        division_res->mantissa_sign = '-';
    else if (first_num.mantissa_sign == '-' && second_num.mantissa_sign == '+')
        division_res->mantissa_sign = '-';
    else
        division_res->mantissa_sign = '+';

    if (!check_null(second_num))
        return err_second_num_zero;

    if (!check_null(first_num))
    {
        division_res->power = 0;
        division_res->power_sign = '+';
        division_res->mantissa_sign= '+';
        for (int i = 0; i < MANTISS_LEN; i++)
            division_res->mantissa_arr[i] = 0;
        return result;
    }

    if (compare_nums(first_num, second_num) == 0)
    {
        for (int i = 0; i < MANTISS_LEN; i++)
            division_res->mantissa_arr[i] = 0;
        division_res->mantissa_arr[0] = 1;
        division_res->power = 1;
        return result;
    }

    while (first_num.mantissa_arr[0] == 0)
        first_num = move_mantiss_left(first_num);

    while (second_num.mantissa_arr[0] == 0)
        second_num = move_mantiss_left(second_num);
    
    division_res->power = first_num.power - second_num.power + 1;
    // Переполнение степени
    if (division_res->power > MAX_POWER)
        result = err_power_overflow;

    // Инициализация структуры результата
    for (int i = 0; i < MANTISS_LEN; i++)
        division_res->mantissa_arr[i] = 0;

    if (compare_nums(first_num, second_num) == -1)
        division_res->power -= 1;

    while (check_del(first_num, second_num, *division_res) && result == execution_complete && i_div < MANTISS_LEN)
    {
        if (compare_nums(first_num, second_num) > -1)
        {
            // Первое больше второго
            // Находим множитель   
            del = seek_del(first_num, second_num);
            if (del == -1)
            {
                if (second_num.mantissa_arr[0] == 0)
                    second_num = move_mantiss_left(second_num);
                del = 1;
            }
            first_num = minus_nums(first_num, multiply_simple(second_num, del));
            division_res->mantissa_arr[i_div] = del;
            i_div++;
        }
        else 
        {
            // Первое меньше второго
            if (first_num.mantissa_arr[0] == 0)
            {
                first_num = move_mantiss_left(first_num);
                if (compare_nums(first_num, second_num) == -1)
                    i_div++;
            }
            else
            {
                second_num = move_mantiss_right(second_num);
            }
        }
    }
    

    // Переполнение степени
    if (abs(division_res->power) > MAX_POWER)
        result = err_power_overflow;
    // Проверка на округление
    if (check_null(first_num))
    {
        first_num = move_mantiss_left(first_num);
        *division_res = mantiss_round(*division_res, seek_del(first_num, second_num));
    }
    return result;
}