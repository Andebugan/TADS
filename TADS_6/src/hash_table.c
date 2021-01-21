#define __USE_MINGW_ANSI_STDIO 1
#include "hash_table.h"

size_t key_div(char *string, size_t table_size)
{
    size_t sum = 0;
    for (size_t i = 0; *(string + i) != '\0'; sum += *(string + i), i++)
        ;
    return sum % table_size;
}

size_t Jenkins_hash(char *s, size_t table_size)
{
    size_t hash = 0;

    while (*s)
    {
        hash += *s++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % table_size;
}

size_t hash_table_fill(hash_linked_t **table, size_t size, buff_t *buff, size_t (*hash_key)(char *, size_t))
{
    int64_t time = 0;
    size_t clsns = 0;

    for (size_t i = 0; i < buff->size; i++)
    {
        size_t cur_clsns = 0;
        if (!unique_check(buff->buff[i], (*table)[hash_key(buff->buff[i], size)]))
        {
            cur_clsns += list_insert(&(*table)[hash_key(buff->buff[i], size)], buff->buff[i], &time);
        }

        clsns += cur_clsns;
    }

    return clsns / size;
}

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

void free_lnked_hash(hash_linked_t *node)
{
    if (node == NULL)
        return;

    free_lnked_hash(node->next);

    free(node);
}

void free_table(hash_linked_t *table, int size)
{
    for (int i = 0; i < size; i++)
    {
        free_lnked_hash(table[i].next);
    }

    free(table);
}