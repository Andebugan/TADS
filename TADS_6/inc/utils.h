#ifndef _UTILS_H_
#define _UTILS_H_

#include "hash_table.h"
#include "tree_funcs.h"
#include "in_out.h"
#include "exit_codes.h"
#include "timer.h"
#include "linked_list.h"

int file_insert(const char *fname, char *string, int64_t *time);
int64_t add_unbalanced(tree_uns_node_t **tree, char *key, long *comp);
int64_t add_elem_balanced(tree_node_t **balanced, char *key, long *comp);

#endif