#ifndef _TREE_FUNCS_H_
#define _TREE_FUNCS_H_

#include <stdlib.h>
#include <string.h>
#include "in_out.h"
#include "exit_codes.h"
#include "timer.h"

#define INDENT 5

typedef struct tree_node tree_node_t;
struct tree_node
{
    char *key;
    long height;
    tree_node_t *left;
    tree_node_t *right;
};

tree_node_t *insert(tree_node_t *node, char *key, long *height, long *comp);
tree_node_t *root_balance(tree_node_t *root);
tree_node_t *add_balanced(tree_node_t *tree, char *key, long *height, int *comp);
int fill_tree(tree_node_t **node, buff_t buff, int64_t *time);
int fill_balanced(tree_node_t **tree, buff_t buff, int64_t *time);
void print_tree(tree_node_t *node, size_t tab, int is_left, int is_right, unsigned int notch, int is_root);

#endif