#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "colors.h"
#include "timer.h"

typedef struct hash_linked hash_linked_t;
struct hash_linked
{
    char *key;
    hash_linked_t *next;
};

int unique_check(char *src, hash_linked_t head);
size_t list_insert(hash_linked_t *table, char *src, int64_t *time);

#endif