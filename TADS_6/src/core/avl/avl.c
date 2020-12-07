#include "./avl.h"
#include <assert.h>

static int __create_leaf(struct avl **leaf, int data)
{
    *leaf = callocs_malloc(sizeof(struct avl));
    if (!*leaf)
        return -1;
    (*leaf)->depth = 1;
    (*leaf)->data = data;
    (*leaf)->count = 1;
    (*leaf)->left = NULL;
    (*leaf)->right = NULL;
    return 0;
}

struct avl avl_create(void)
{
    struct avl tree = { .depth = 0, .left = NULL, .right = NULL };
    return tree;
}

static void __rc_destroy(struct avl *tree)
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

void avl_destroy(struct avl *tree)
{
    __rc_destroy(tree);
    tree->depth = 0;
}

static void __right_short_rotate(struct avl *tree)
{
    if (tree->left->diff == -1)
        __right_short_rotate(tree->left);

    struct avl *node = tree->left;
    while (node->right->right)
        node = node->right;

    int data = tree->data;
    int count = tree->count;

    data = data;
    count = count;

    // tree->data = ...;
    assert(0 && "not implemented");
}

static void __right_wide_rotate(struct avl *tree)
{
    tree = tree;
}

static void __left_short_rotate(struct avl *tree)
{
    tree = tree;
}

static void __left_wide_rotate(struct avl *tree)
{
    tree = tree;
}

static void __balance_tree(struct avl *tree)
{
    switch (tree->diff)
    {
        case -1:
        case 0:
        case 1:
            break;

        case -2:
            assert(tree->left && "bad left part when balance");
            if (tree->left->right)
                __right_short_rotate(tree);
            else
                __right_wide_rotate(tree);
            break;

        case 2:
            assert(tree->right && "bad right part when balance");
            if (tree->right->left)
                __left_short_rotate(tree);
            else
                __left_wide_rotate(tree);
            break;

        default:
            assert(0 && "bad tree->diff when balance");
            break;
    }
}

int avl_insert(struct avl *tree, int data)
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
            status = avl_insert(tree->left, data);
            tree->depth = tree->depth > 1 + tree->right->depth ?
                tree->depth : (1 + tree->right->depth);
            if (tree->right)
                tree->diff = tree->right->depth - tree->left->depth;
            else
                tree->diff = -tree->left->depth;
        }
        else
        {
            status = __create_leaf(&tree->left, data);
            tree->depth = tree->depth == 1 ? 2 : tree->depth;
            tree->diff--;
        }
    }
    else
    {
        if (tree->right)
        {
            status = avl_insert(tree->right, data);
            tree->depth = tree->depth > 1 + tree->right->depth ?
                tree->depth : (1 + tree->right->depth);
            if (tree->left)
                tree->diff = tree->right->depth - tree->left->depth;
            else
                tree->diff = tree->right->depth;
        }
        else
        {
            status = __create_leaf(&tree->right, data);
            tree->depth = tree->depth == 1 ? 2 : tree->depth;
            tree->diff++;
        }
    }

    if (tree->diff < -1 || tree->diff > 1)
        __balance_tree(tree);

    return status;
}

// возвращает -1 если элемента нет, и 0 - если успешно удалён.
int avl_remove(struct avl *tree, int data);

// NULL - если не найдено.
struct avl *avl_find(struct avl *tree, int data);
