#define __USE_MINGW_ANSI_STDIO 1
#include "in_out.h"
#include "exit_codes.h"
#include "timer.h"
#include "colors.h"

void greets()
{
    puts(YELLOW "<Лабораторная работа №6>" RESET);
}

int fill_buff(const char *fname, buff_t *buff)
{
    FILE *source = fopen(fname, "r");
    if (source == NULL)
        return INVALID_FNAME;

    buff_string cur;
    size_t cur_size = 0;
    while (fgets(cur, DEFAULT, source) != NULL)
    {
        if (cur[strlen(cur) - 1] == '\n')
            cur[strlen(cur) - 1] = '\0';
        else
            cur[strlen(cur)] = '\0';
        strcpy(buff->buff[cur_size++], cur);
    }
    if (cur_size == 0)
        return EMPTY_SOURCE;
    buff->size = cur_size;

    fclose(source);
    return EXIT_SUCCESS;
}

void main_menu()
{
    puts(CYAN "<Главное меню>" RESET);
    puts(YELLOW "1 - Работа с бинарным деревом поиска");
    puts("2 - Работа с хэш-таблицей");
    puts("3 - Замер времени вставки элемента в файл *отладка*");
    puts("Для выхода из программы нажмите любую другую клавишу..." RESET);
}

void hash_menu()
{
    puts("");
    puts(YELLOW "1 - Построить хэш-таблицу по данным из файла");
    puts("2 - Вывести таблицу на экран");
    puts("3 - Добавить слово в хэш-таблицу");
    puts("Для выхода в главное меню нажмите любую другую клавишу..." RESET);
    puts("");
}

void tree_menu()
{
    puts("");
    puts(YELLOW "1 - Построить ДДП (двоичное дерево поиска)");
    puts("2 - Сбалансировать ДДП");
    puts("3 - Добавить слово в несбалансированное ДДП");
    puts("4 - Добавить слово в сбалансированное ДДП");
    puts("5 - Сравнить время добавления слов в несбалансированное и\n сбалансированное ДДП");
    puts("Для выхода в главное меню нажмите любую другую клавишу..." RESET);
    puts("");
}

void get_fname()
{
    puts("");
    puts(CYAN "Имя файла вводится вместе с расширением. Если файл находится");
    puts("в поддиректории, то следует ввести имя в формате /DIR/file.txt" RESET);
    printf(YELLOW "Введите имя файла, из которого будут прочитаны данные : " RESET);
    puts("");
}

void clean_input_stream()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}