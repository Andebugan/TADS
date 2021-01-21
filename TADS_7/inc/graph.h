#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "matrix.h"

void remove_edge(matrix_t *mtx, size_t row);

bool is_cycle(size_t u, bool visited[], size_t parent, matrix_t graph);

bool is_tree(matrix_t graph);

#endif // _GRAPH_H_