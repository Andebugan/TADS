#include "./bst.h"

static int __create_leaf(struct bst **leaf, int data)
{
    *leaf = callocs_malloc(sizeof(struct bst));
    if (!*leaf)
        return -1;
    (*leaf)->depth = 1;
    (*leaf)->data = data;
    (*leaf)->count = 1;
    (*leaf)->left = NULL;
    (*leaf)->right = NULL;
    return 0;
}

struct bst bst_create(void)
{
    struct bst tree = { .depth = 0, .left = NULL, .right = NULL };
    return tree;
}

static void __rc_destroy(struct bst *tree)
{
    if (tree->left)
    {
        __rc_destroy(tree->left);
        callocs_free(tree->left);
    }
    if (tree->right)
    {
        __rc_destroy(tree->right);
        callocs_free(tree->right);
    }
}

void bst_destroy(struct bst *tree)
{
    __rc_destroy(tree);
    tree->depth = 0;
}

int bst_insert(struct bst *tree, int data)
{
    int status = 0;

    if (tree->depth == 0)
    {
        tree->data = data;
        tree->count = 1;
        tree->depth = 1;
    }
    else if (data == tree->data)
        tree->count++;
    else if (data < tree->data)
    {
        if (tree->left)
        {
            status = bst_insert(tree->left, data);
            tree->depth = tree->depth > 1 + tree->left->depth ?
                tree->depth : (1 + tree->left->depth);
        }
        else
        {
            status = __create_leaf(&tree->left, data);
            tree->depth = tree->depth == 1 ? 2 : tree->depth;
        }
    }
    else
    {
        if (tree->right)
        {
            status = bst_insert(tree->right, data);
            tree->depth = tree->depth > 1 + tree->right->depth ?
                tree->depth : (1 + tree->right->depth);
        }
        else
        {
            status = __create_leaf(&tree->right, data);
            tree->depth = tree->depth == 1 ? 2 : tree->depth;
        }
    }

    return status;
}

int bst_remove(struct bst *tree, int data)
{
    int status = 0;

    if (tree->depth-- > 0) // expect found
    {
        if (tree->data == data)
        {
            if (tree->count > 1)
            {
                tree->count--;
                tree->depth++;
            }
        }
        else if (data < tree->data && tree->left)
        {
            status = bst_remove(tree->left, data);
            if (tree->left->depth == 0)
            {
                callocs_free(tree->left);
                tree->left = NULL;
            }
        }
        else if (tree->data < data && tree->right)
        {
            status = bst_remove(tree->right, data);
            if (tree->right->depth == 0)
            {
                callocs_free(tree->right);
                tree->right = NULL;
            }
        }
    }
    else
        status = -1;

    if (status == -1)
        tree->depth++; // not found

    return status;
}

struct bst *bst_find(struct bst *tree, int data)
{
    if (tree->depth == 0)
        return NULL;
    else if (tree->data == data)
        return tree;
    else if (data < tree->data)
    {
        if (tree->left)
            return bst_find(tree->left, data);
        else
            return NULL;
    }
    else if (tree->data < data)
    {
        if (tree->right)
            return bst_find(tree->right, data);
        else
            return NULL;
    }
    return NULL;
}

#define PRN_DATA_WIDTH 5

static int *__get_by_path(struct bst *tree, long path, int len)
{
    if (len == 0)
        return &tree->data;
    else if ((path & (1 << (len - 1))) == 0 && tree->left)
        return __get_by_path(tree->left, path, len - 1);
    else if ((path & (1 << (len - 1))) == 1 && tree->right)
        return __get_by_path(tree->right, path, len - 1);
    else
        return NULL;
}

static void __print_next_level(FILE *file, struct bst *tree, int level)
{
    if (tree->depth - level > 1)
        for (int i = 0; i < (1 << (tree->depth - level - 2)); i++)
            fprintf(file, "%*s", (PRN_DATA_WIDTH + 1) / 2, "");

    for (long path = 0; path < (1 << level); path++)
    {
        int *data = __get_by_path(tree, path, level);
        if (data)
            fprintf(file, "%0*d ", PRN_DATA_WIDTH, *data);
        else
            fprintf(file, "%*s ", PRN_DATA_WIDTH, "");
        
        // make callocs_free space between
        if (tree->depth - level > 1)
            for (int i = 0; i < (1 << (tree->depth - level - 1)); i++)
                fprintf(file, "%*s ", PRN_DATA_WIDTH, "");
    }

    fprintf(file, "\n");

    // recursion case
    if (level < tree->depth)
        __print_next_level(file, tree, level + 1);
}

void bst_fprintf(FILE *file, struct bst *tree)
{
    fprintf(file, "tree depth = %d\n", tree->depth);

    // print level 0
    if (tree->depth > 1)
        for (int i = 0; i < (1 << (tree->depth - 2)); i++)
            fprintf(file, "%*s", (PRN_DATA_WIDTH + 1) / 2, "");
    fprintf(file, "%0*d\n", PRN_DATA_WIDTH, tree->data);

    // print next levels
    __print_next_level(file, tree, 1);
}
