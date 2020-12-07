#include "func.h"

/**
 * Это объявление типа operation_status, который показывает, 
 * что именно сейчас обрабатывает функция считывания числа.
 */
typedef enum
{
    processing_sign,
    processing_zeros_before_mantiss,
    processing_mantiss_before_point,
    processing_mantiss_after_point,
    processing_power_char,
    processing_power_sign,
    processing_power_num,
    processing_after_num
} operation_status;

/**
 * Эта функция обрабатывает знак мантиссы
 */
error_status sign_process(char sign_char, operation_status *status, large_num *num, int *mantiss_num, int *power_sum)
{
    error_status result = execution_complete;
    char num_arr[10] = "123456789";
    // 1) пробел
    if (sign_char == ' ')
    ;
    // 2) первый + или -
    else if (sign_char == '+' || sign_char == '-')
    {
        num->mantissa_sign = sign_char;
        *status = processing_zeros_before_mantiss;
    }
    else if (sign_char == '0')
        *status = processing_zeros_before_mantiss;
    // 3) корректный переход к обработке числа
    else if (strchr(num_arr, sign_char) != NULL)
    {
        num->mantissa_sign = '+';
        (num->mantissa_arr)[*mantiss_num] = sign_char - '0';
        (*mantiss_num)++;
        (*power_sum)++;
        *status = processing_mantiss_before_point;
    }
    // 4) обработка первой точки
    else if (sign_char == '.')
    {
        num->mantissa_sign = '+';
        *status = processing_mantiss_after_point;
    }
    // 5) ошибочный ввод
    else
        result = err_sign_incorrect;
    return result;
}
/** 
 * Функция обработки нулей до мантиссы
 */
error_status zeros_before_mantiss_process(char zeros_char, operation_status *status, large_num *num, int *mantiss_num, int *power_sum)
{
    error_status result = execution_complete;
    char num_arr[10] = "1234567890";
    // 1) незначащий ноль
    if (zeros_char == '0')
    ;
    else if (zeros_char == '\0' || zeros_char == '\r' || zeros_char == '\n')
        *status = processing_after_num;
    // 2) переход к обработке числа
    else if (strchr(num_arr, zeros_char) != NULL)
    {
        if (num->mantissa_sign != '-')
            num->mantissa_sign = '+';
        (num->mantissa_arr)[*mantiss_num] = zeros_char - '0';
        (*mantiss_num)++;
        (*power_sum)++;
        *status = processing_mantiss_before_point;
    }
    // 3) точка
    else if (zeros_char == '.')
    {
        num->mantissa_sign = '+';
        *status = processing_mantiss_after_point;
    }
    // 4) некорректный символ
    else
        result = err_mantiss_incorrect;
    return result;
}

/**
 * Обработка мантиссы перед точкой
 */
error_status process_mantiss_before_point(char mantissa_char, operation_status *status, large_num *num, int *power_sum, int *mantiss_num)
{
    error_status result = execution_complete;
    char num_arr[10] = "1234567890";
    // Конец числа
    if (mantissa_char == '\0' || mantissa_char == '\r' || mantissa_char == '\n')
        *status = processing_after_num;
    // если точка, то переход к обратотке числе после точки
    else if (mantissa_char == '.')
        *status = processing_mantiss_after_point;
    // запись числа в мантиссу
    else if (strchr(num_arr, mantissa_char) != NULL)
    {
        (num->mantissa_arr)[*mantiss_num] = mantissa_char - '0';
        (*mantiss_num)++;
        (*power_sum)++;
    }
    // Конец записи мантиссы
    else if (mantissa_char == ' ')
        *status = processing_power_char;
    // некорректный символ
    else
        result = err_mantiss_incorrect;
    return result;
}

/**
 * Обработка мантиссы после точки
 */
error_status process_mantiss_after_point(char mantissa_char, operation_status *status, large_num *num, int *mantiss_num)
{
    error_status result = execution_complete;
    char num_arr[10] = "1234567890";
    // 1) конец числа
    if (mantissa_char == '\0' || mantissa_char == '\r' || mantissa_char == '\n')
        *status = processing_after_num;
    // 2) конец мантиссы
    else if (mantissa_char == ' ' || mantissa_char == 'E' || mantissa_char == 'e')
        *status = processing_power_char;
    // 3) запись числа в мантиссу
    else if (strchr(num_arr, mantissa_char) != NULL)
    {
        (num->mantissa_arr)[*mantiss_num] = mantissa_char - '0';
        (*mantiss_num)++;
    }
    // 4) некорректный символ
    else
        result = err_mantiss_incorrect;
    return result;
}

/**
 * Обработка символа степени
 */
error_status process_power_char(char power_char, operation_status *status, large_num *num)
{
    error_status result = execution_complete;
    // 1) Пробел
    if (power_char == ' ')
    ;
    // 2) Первый e или E
    else if (power_char == 'e' || power_char == 'E')
        *status = processing_power_sign;
    // 3) Конец числа
    else if (power_char == '\0' || power_char == '\r' || power_char == '\n')
        *status = processing_after_num;
    // 4) Ошибочный ввод
    else
        result = err_sign_incorrect;
    return result;
}

/**
 * Добавление числа в число степени
 */
error_status process_num_power(large_num *num, int power_num, char power_char)
{
    error_status result = execution_complete;
    if (power_num > 5)
        result = err_power_overflow;
    else
    {
        num->power *= 10;
        num->power += power_char - '0';
    }
    
    return result;
}

/**
 * Обработка знака степени
 */
error_status process_power_sign(char power_char, operation_status *status, large_num *num, int *power_num)
{
    error_status result = execution_complete;
    char num_arr[10] = "1234567890";
    // 1) Пробел
    if (power_char == ' ')
    ;
    // 2) первый + или -
    else if (power_char == '+' || power_char == '-')
    {
        num->power_sign = power_char;
        *status = processing_power_num;
    }
    // 3) корректный переход к обработке числа
    else if (strchr(num_arr, power_char) != NULL)
    {
        if (num->power_sign != '-');
            num->power_sign = '+';
        (*power_num)++;
        result = process_num_power(num, *power_num, power_char);
        *status = processing_power_num;
    }
    // 4) ошибочный ввод
    else
        result = err_power_sign_incorrect;
    return result;
}

/**
 * Обработка числа степени
 */
error_status process_power_num(char power_char, operation_status *status, large_num *num, int *power_num)
{
    error_status result = execution_complete;
    char num_arr[10] = "1234567890";
    // 1) пробел
    if (power_char == ' ' || power_char == '\0' || power_char == '\r' || power_char == '\n')
        *status = processing_after_num;
    // 2) запись числа в степень
    if (strchr(num_arr, power_char) != NULL)
    {
        (*power_num)++;
        result = process_num_power(num, *power_num, power_char);
    }
    // 3) некорректный символ
    else
        result = err_power_incorrect;
    return result;
}

void print_num(large_num division_res)
{
    int mantiss_len = 1;
    for (int i = 0; i < MANTISS_LEN; i++)
    {
        if (division_res.mantissa_arr[i] != 0)
            mantiss_len = i + 1;
    }
    printf("Результат: %c0.", division_res.mantissa_sign);
    for (int i = 0; i < mantiss_len; i++)
        printf("%hi", division_res.mantissa_arr[i]);
    if (division_res.power < 0)
        printf(" E%i\n", division_res.power);
    else 
        printf(" E+%i\n", division_res.power);
}

/**
 * Эта функция считывания одного числа из консоли. На вход она получает
 * указатель на структуру данных, которую необходимо обработать. Функция считывает
 * данные и преобразует строку, которую ввёл пользователь, в поля структуры.
 */
error_status read_num(large_num *num)
{
    error_status result = execution_complete;
    operation_status status = processing_sign;
    int power_len = 0;
    int power_sum = 0;
    int mantiss_len = 0;
    char input_buff[MAX_INPUT_LEN];

    // Инициализация поля знака структуры
    num->mantissa_sign = ' ';
    num->power_sign = ' ';

    // Инициализация мантиссы и степени структуры
    for (int i = 0; i < MANTISS_LEN + 1; i++)
        (num->mantissa_arr)[i] = 0;
    num->power = 0;

    // Ввод данных с консоли
    if (fgets(input_buff, MAX_INPUT_LEN, stdin) != input_buff)
        result = err_mantiss_too_big;

    
    //Обработка строки
    if (result == execution_complete)
    {
        for (int i = 0; i < strlen(input_buff) - 1; i++)
        {
            // обработка знака
            if (result == execution_complete && status == processing_sign)
                result = sign_process(input_buff[i], &status, num, &power_sum, &mantiss_len);
            // обработка нулей до мантиссы
            else if (result == execution_complete && status == processing_zeros_before_mantiss)
                result = zeros_before_mantiss_process(input_buff[i], &status, num, &power_sum, &mantiss_len);
            // обработка мантиссы до точки
            else if (result == execution_complete && status == processing_mantiss_before_point)
                result = process_mantiss_before_point(input_buff[i], &status, num, &power_sum, &mantiss_len);
            // обработка мантиссы после точки
            else if (result == execution_complete && status == processing_mantiss_after_point)
                result = process_mantiss_after_point(input_buff[i], &status, num, &mantiss_len);
            // обработка символа степени
            else if (result == execution_complete && status == processing_power_char)
                result = process_power_char(input_buff[i], &status, num);
            // обработка символа степени
            else if (result == execution_complete && status == processing_power_sign)
                result = process_power_sign(input_buff[i], &status, num, &power_len);
            // обработка числа степени
            else if (result == execution_complete && status == processing_power_num)
                result = process_power_num(input_buff[i], &status, num, &power_len);
            // обработка конца строки
            else if (result == execution_complete && status == processing_after_num)
                break;
            if (result != execution_complete)
                break;
            // проверка на длинну мантиссы
            if (mantiss_len > 30)
                result = err_mantiss_too_big;
            if (power_len > 5)
                result = err_power_incorrect;
        }

        if (result == execution_complete)
        {
            if (num->power_sign == '-')
                num->power *= -1;
            num->power += power_sum;
        }
    }
    return result;
}

error_status read_nums(large_num *first_num, large_num *second_num)
{
    error_status result = execution_complete;
    printf("Введите ниже первое число:\n");
    result = read_num(first_num);
    if (result == execution_complete)
    {
        printf("Введите ниже второе число:\n");
        result = read_num(second_num);
    }
    return result;
}