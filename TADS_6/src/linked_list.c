#include "linked_list.h"

//Проверка на уникальноть элемента хеш-таблицы при создании
int unique_check(char *src, hash_linked_t head)
{
    hash_linked_t *cur = &head;
    int i = 0;

    for (; cur && cur->key; cur = cur->next)
    {
        i++;
        if (strcmp(cur->key, src) == 0)
            return i;
    }

    return 0;
}

// Добавляем новый элемент в связный список с замером времени
size_t list_insert(hash_linked_t *table, char *src, int64_t *time)
{
    int64_t start = 0;
    *time = 0;
    short int existing = 0;
    // Глубина связного списка
    size_t depth = 1;

    hash_linked_t *cur = table;
    // Создаём ячейку, если она не создана и добавляем туда данные
    if (!table)
    {
        table = calloc(1, sizeof(hash_linked_t));
        table->key = calloc((strlen(src) + 1), sizeof(char));
        strcpy(table->key, src);
        return 1;
    }

    // Проходим до конца связного списка
    start = proc_tick();
    for (; cur->next; cur = cur->next, depth++)
    {
        if (strcmp(cur->key, src) == 0)
            existing = 1;
    }
    *time += proc_tick() - start;

    // Добавляем элемент в конец связного списка
    if (existing == 0)
    {
        cur->next = calloc(1, sizeof(hash_linked_t));
        cur->key = calloc((strlen(src) + 1), sizeof(char));
        start = proc_tick();
        strcpy(cur->key, src);
        cur = cur->next;
        cur->next = NULL;
        *time += proc_tick() - start;
    }
    else
        printf(YELLOW "Данное значение уже занесено в хеш-таблицу.\n" RESET);
    
    return depth;
}
