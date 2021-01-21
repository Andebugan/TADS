#define __USE_MINGW_ANSI_STDIO 1
#include "utils.h"

int file_insert(const char *fname, char *string, int64_t *time)
{
    *time = 0;
    FILE *src = fopen(fname, "r+");
    if (!src)
        return INVALID_FNAME;
    int64_t start = proc_tick();
    fseek(src, 0L, SEEK_END);
    fprintf(src, "%s", string);
    *time = proc_tick() - start;
    fclose(src);

    return EXIT_SUCCESS;
}

int64_t add_unbalanced(tree_node_t **tree, char *key)
{
    long comp = 0;
    long height = -1;

    int64_t start, end;
    start = proc_tick();
    *tree = insert(*tree, key, &height, &comp);
    end = proc_tick() - start;

    return end;
}

int64_t add_elem_balanced(tree_node_t **balanced, char *key)
{
    int64_t start, end;
    long height = -1;
    int comp = 0;

    start = proc_tick();
    *balanced = add_balanced(*balanced, key, &height, &comp);
    end = proc_tick() - start;

    return end;
}
