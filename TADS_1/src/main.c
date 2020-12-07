#include "func.h"
#include "in_out.h"
#include "num_funcs.h"

int main()
{
    error_status result = execution_complete;
    large_num first_num;
    large_num second_num;
    large_num division_res;

    // Инициализация структур
    for (int i = 0; i < MANTISS_LEN; i++)
        first_num.mantissa_arr[i] = 0;
    for (int i = 0; i < MANTISS_LEN; i++)
        second_num.mantissa_arr[i] = 0;

    first_num.mantissa_sign = '+';
    first_num.power_sign = '+';
    first_num.power = 0;

    second_num.mantissa_sign = '+';
    second_num.power_sign = '+';
    second_num.power = 0;

    printf("Программа деления двух многозначных чисел.\n");
    printf("Для того, чтобы произвести деление, вам необходимо ввести два числа в виде:");
    printf("+/-m.n E+/-K\nгде:\n");
    printf("m + n - Суммарная длина мантиссы, не более 30 значащих цифр, m и n вводятся с клавиатуры\n");
    printf("E - символ степени числа, допускается запись в виде e\n");
    printf("K - число степени мантиссы, величина порядка K не более 5 (не более 99999)\n");
    
    // Ввод чисел
    result = read_nums(&first_num, &second_num);
    // Обработка чисел
    if (result == execution_complete)
    {
        result = divide_nums(first_num, second_num, &division_res);
    }
    // Вывод
    if (result == execution_complete)
        print_num(division_res);
    if (result != execution_complete)
    {
        if (result == 1)
            printf("Ошибка - знак мантиссы был введён неверно!");
        if (result == 2)
            printf("Ошибка - знак степени был введён неверно!");
        if (result == 3)
            printf("Ошибка - превышена максимальная длина мантиссы!");
        if (result == 4)
            printf("Ошибка - в мантиссе обнаружены символы, не являющиеся числами!");
        if (result == 5)
            printf("Ошибка - знак экспоненты записан неверно!");
        if (result == 6)
            printf("Ошибка - в степени обнаружены символы, не являющиеся числами!");
        if (result == 7)
            printf("Ошибка - превышена максимальная длина степени!");
        if (result == 8)
            printf("Ошибка - возникла ошибка в процессе деления!");
        if (result == 9)
            printf("Ошибка - деление на ноль!");
    }
    return result;

}