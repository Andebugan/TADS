#define __USE_MINGW_ANSI_STDIO 1
#include "../inc/graph.h"

void remove_edge(matrix_t *mtx, size_t row)
{
    delete_row(mtx, row);
    transpose(mtx);
    delete_row(mtx, row);
}

bool is_cycle(size_t u, bool visited[], size_t parent, matrix_t graph)
{
    visited[u] = true;
    for (size_t v = 0; v < graph.column_num; v++)
    {
        if (graph.value[u][v])
        {
            if (!visited[v])
            {
                if (is_cycle(v, visited, u, graph))
                    return true;
            }
            else if (v != parent)
                return true;
        }
    }
    return false;
}

bool is_tree(matrix_t graph)
{
    bool visited[graph.column_num];

    for (size_t i = 0; i < graph.column_num; i++)
        visited[i] = false;

    if (is_cycle(0, visited, graph.column_num + 1, graph))
        return false;

    for (size_t i = 0; i < graph.column_num; i++)
        if (!visited[i])
            return false;

    return true;
}

int export_to_dot(char *filename, matrix_t graph, size_t deleted)
{
    FILE *dst = fopen(filename, "w");
    if (!dst)
        return EXIT_FAILURE;

    fprintf(dst, "graph graphname {\n");
    for (size_t i = 0; i < graph.column_num; i++)
        if (i != deleted)
            fprintf(dst, "\"%zu\";\n", i);

    for (size_t i = 0; i < graph.column_num; i++)
        for (size_t j = 0; j < i; j++)
        {
            if (i != deleted && j != deleted)
                if (graph.value[i][j])
                    fprintf(dst, "\"%zu\" -- \"%zu\";\n", i, j);
        }
    fprintf(dst, "}");
    fclose(dst);

    return EXIT_SUCCESS;
}