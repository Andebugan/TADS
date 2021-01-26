#include "tree_funcs.h"

int tree_node_init(tree_node_t **node, char *string, long height)
{
    *node = malloc(sizeof(tree_node_t));
    if (!*node)
        return ALLOCATION_FAILURE;
    (*node)->key = malloc((strlen(string) + 1) * sizeof(char));
    if (!(*node)->key)
        return ALLOCATION_FAILURE;
    strcpy((*node)->key, string);

    (*node)->left = NULL;
    (*node)->right = NULL;

    (*node)->height = height;
    return EXIT_SUCCESS;
}

int get_height(tree_node_t *tree)
{
    return tree ? tree->height : 0;
}

int balance_factor(tree_node_t *tree)
{
    return get_height(tree->right) - get_height(tree->left);
}

tree_node_t *insert(tree_node_t *node, char *key, long *height, long *comp)
{
    (*height)++;
    (*comp)++;

    if (!node)
        tree_node_init(&node, key, *height);
    else if (strcmp(key, node->key) < 0)
        node->left = insert(node->left, key, height, comp);
    else if (strcmp(key, node->key) > 0)
        node->right = insert(node->right, key, height, comp);

    return node;
}

int fill_tree(tree_node_t **node, buff_t buff, int64_t *time)
{
    *time = 0;
    int64_t start;
    for (size_t i = 0; i < buff.size; i++)
    {
        long comp = 0;
        long height = -1;

        start = proc_tick();
        *node = insert(*node, buff.buff[i], height, &comp);
        *time += proc_tick() - start;
    }
    return EXIT_SUCCESS;
}

void fix_height(tree_node_t *tree)
{
    int h_left = get_height(tree->left);
    int h_right = get_height(tree->right);

    tree->height = (h_left > h_right ? h_left : h_right) + 1;
}

tree_node_t *rotate_left(tree_node_t *root)
{
    tree_node_t *new_root = root->right;

    root->right = new_root->left;
    new_root->left = root;

    fix_height(root);
    fix_height(new_root);

    return new_root;
}

tree_node_t *rotate_right(tree_node_t *root)
{
    tree_node_t *new_root = root->left;

    root->left = new_root->right;
    new_root->right = root;

    fix_height(root);
    fix_height(new_root);

    return new_root;
}

tree_node_t *add_balanced(tree_node_t *tree, char *key, long *height, int *comp)
{
    (*height)++;
    (*comp)++;

    if (!tree)
        tree_node_init(&tree, key, *height);
    else if (strcmp(key, tree->key) < 0)
        tree->left = add_balanced(tree->left, key, height, comp);
    else if (strcmp(key, tree->key) > 0)
        tree->right = add_balanced(tree->right, key, height, comp);

    return root_balance(tree);
}

tree_node_t *root_balance(tree_node_t *root)
{
    fix_height(root);

    if (balance_factor(root) == 2)
    {
        if (balance_factor(root->right) < 0)
            root->right = rotate_right(root->right);
        return rotate_left(root);
    }
    if (balance_factor(root) == -2)
    {
        if (balance_factor(root->left) > 0)
            root->left = rotate_left(root->left);

        return rotate_right(root);
    }

    return root;
}

int fill_balanced(tree_node_t **tree, buff_t buff, int64_t *time)
{
    *time = 0;
    int64_t start;
    for (size_t i = 0; i < buff.size; i++)
    {
        long height = -1;

        int comp = 0;

        start = proc_tick();
        *tree = add_balanced(*tree, buff.buff[i], &height, &comp);
        *time += proc_tick() - start;
    }

    return EXIT_SUCCESS;
}

void print_tree(tree_node_t *node, size_t tab, int is_left, int is_right, unsigned int notch, int is_root)
{
    if (node)
    {
        print_tree(node->left, tab + 1, 1, 0, (is_left ? (notch << 1) : (notch << 1) | 1), 0);

        for (size_t i = 0; i < tab; i++)
        {
            if ((i != 0) && (notch & (1U << (tab - i - 1))))
                printf("|");
            else
                printf("  ");
            printf("%*s", INDENT, "");
        }

        printf("%s", (is_left ? "┌─" : (is_right ? "└─" : "")));

        int n = snprintf(NULL, 0, "%s", node->key);
        printf("%.*s", (INDENT - n) * 3, "―――――");
        printf(" %s ", node->key);

        if (is_root)
        {
            printf("├─");
        }

        printf("\n");

        print_tree(node->right, tab + 1, 0, 1, (is_right ? (notch << 1) : (notch << 1) | 1), 0);
    }
}