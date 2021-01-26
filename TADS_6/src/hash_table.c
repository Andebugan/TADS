#define __USE_MINGW_ANSI_STDIO 1
#include "hash_table.h"

// Простая функция хеширования на основе суммы кодов символов
size_t key_div(char *string, size_t table_size)
{
    size_t sum = 0;
    // Суммирование кодов символов
    for (size_t i = 0; *(string + i) != '\0'; sum += *(string + i), i++)
        ;
    // Деление на размер таблицы
    return sum % table_size;
}

// Битовая хеш-функция
size_t bit_hash(char *s, size_t table_size)
{
    // Хеш-код
    size_t hash = 0;
    // Для всех символов строки
    while (*s)
    {
        hash += *s++;
        hash += (hash << 1);
        hash ^= (hash << 1);
    }
    hash += (hash << 1);
    hash ^= (hash << 1);
    return hash % table_size;
}

// Заполнение хеш-таблицы
size_t hash_table_fill(hash_linked_t **table, size_t size, buff_t *buff, size_t (*hash_key)(char *, size_t))
{
    int64_t time = 0;
    size_t clsns = 0;

    for (size_t i = 0; i < buff->size; i++)
    {
        size_t cur_clsns = 0;
        // Проверка на наличие элемента в таблице
        if (!unique_check(buff->buff[i], (*table)[hash_key(buff->buff[i], size)]))
        {   
            // Вставка элемента в таблицу
            cur_clsns = list_insert(&(*table)[hash_key(buff->buff[i], size)], buff->buff[i], &time);
        }
        // Количество коллизий
        clsns = cur_clsns > clsns ? cur_clsns: clsns;
    }
    // Самое большое количество коллизий для одного ключа
    return clsns;
}

// Вывод хеш-таблицы
void hash_table_print(hash_linked_t *table, size_t size)
{
    hash_linked_t *cur;
    int word_indent = 20;

    for (size_t i = 0; i < size; i++)
    {
        cur = &table[i];
        if (cur->key)
        {
            printf(CYAN "Хеш-код: " RESET "%zu\n" CYAN "Ключи: " RESET "\n", i);
        }
        for (; cur->next; cur = cur->next)
            printf("%*s\n", word_indent, cur->key);
        if (table[i].key)
            puts("");
    }
}

// Освобождени памяти из-под списка элементов
void free_lnked_hash(hash_linked_t *node)
{
    if (node == NULL)
        return;

    free_lnked_hash(node->next);

    free(node);
}

// Освобождение хеш-таблицы
void free_table(hash_linked_t *table, int size)
{
    for (int i = 0; i < size; i++)
    {
        free_lnked_hash(table[i].next);
    }

    free(table);
}

// Количество элементов в таблице
long int table_elem_col(hash_linked_t *table, int size)
{
    long int count = 0;
    hash_linked_t *cur;
    for (int i = 0; i < size; i++)
    {
        cur = &table[i];
        count += sizeof(hash_linked_t);
        for (; cur->next; cur = cur->next)
            count += sizeof(hash_linked_t);
    }
    return count;
}