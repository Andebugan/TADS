#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <string.h>
#include "in_out.h"
#include "linked_list.h"
#include "colors.h"

size_t key_div(char *string, size_t table_size);
size_t Jenkins_hash(char *s, size_t table_size);
size_t hash_table_fill(hash_linked_t **table, size_t size, buff_t *buff, size_t (*hash_key)(char *, size_t));
void hash_table_print(hash_linked_t *table, size_t size);
void free_lnked_hash(hash_linked_t *node);
void free_table(hash_linked_t *table, int size);

#endif // HASH_TABLE