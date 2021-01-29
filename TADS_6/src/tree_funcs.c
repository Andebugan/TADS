#include "tree_funcs.h"
#include "colors.h"

// Инициализация АВЛ
int tree_node_init(tree_node_t **node, char *string, long height)
{
    // Выделение памяти под корень
    *node = malloc(sizeof(tree_node_t));
    // Ошибка выделения памяти
    if (!*node)
        return ALLOCATION_FAILURE;
    // Выделяем память под данные (ключ)
    (*node)->key = malloc((strlen(string) + 1) * sizeof(char));
    // Ошибка выделения памяти под данные
    if (!(*node)->key)
        return ALLOCATION_FAILURE;
    // Копируем в ключ строку
    strcpy((*node)->key, string);

    // Указатели на потомков
    (*node)->left = NULL;
    (*node)->right = NULL;

    // Высота узла
    (*node)->height = height;
    return EXIT_SUCCESS;
}

// Возвращает высоту дерева
int get_height(tree_node_t *tree)
{
    return tree ? tree->height : 0;
}

// Проверка на необходимость балансировки критерием для АВЛ (hr - hl)
int balance_factor(tree_node_t *tree)
{
    return get_height(tree->right) - get_height(tree->left);
}

// Добавление узла в ДДП
tree_node_t  *insert(tree_node_t *node, char *key, long *height, long *comp)
{
    // Увеличение параметров высоты дерева и количества сравнений
    (*comp)++;
    (*height)++;
    
    // Вставка узла в дерево
    if (!node)
        tree_node_init(&node, key, *height);
    // Если младше чем предок, то влево
    else if (strcmp(key, node->key) < 0)
        node->left = insert(node->left, key, height, comp);
    // Если старше чем предок, то вправо
    else if (strcmp(key, node->key) > 0)
        node->right = insert(node->right, key, height, comp);

    return node;
}

// Заполнение несбалансированного АВЛ
int fill_tree(tree_node_t **node, buff_t buff, int64_t *time)
{
    *time = 0;
    int64_t start;
    for (size_t i = 0; i < buff.size; i++)
    {
        long comp = 0;
        long height = -1;
        start = proc_tick();
        *node = insert(*node, buff.buff[i], &height, &comp);
        *time += proc_tick() - start;
    }
    return EXIT_SUCCESS;
}

// Обновление высоты дерева
void fix_height(tree_node_t *tree)
{
    // Высота левого поддерева
    int h_left = get_height(tree->left);
    // Высота правого поддерева
    int h_right = get_height(tree->right);
    // Выбор максимальной высоты дерева
    tree->height = (h_left > h_right ? h_left : h_right) + 1;
}

// Поворот влево
tree_node_t *rotate_left(tree_node_t *root)
{
    // Новый корень поддерева
    tree_node_t *new_root = root->right;

    // Вращение влево
    root->right = new_root->left;
    new_root->left = root;

    // Обновление высоты дерева
    fix_height(root);
    fix_height(new_root);

    return new_root;
}

// Поворот вправо
tree_node_t *rotate_right(tree_node_t *root)
{
    // Новый корень поддерева
    tree_node_t *new_root = root->left;

    // Вращение вправо
    root->left = new_root->right;
    new_root->right = root;

    // Обновление высоты
    fix_height(root);
    fix_height(new_root);

    return new_root;
}

// Добавление элемента в ДДП
tree_node_t *add_balanced(tree_node_t *tree, char *key, long *height, long *comp)
{
    (*height)++;
    (*comp)++;

    // Инициализируем дерево, если оно отсутствует
    if (!tree)
        tree_node_init(&tree, key, *height);
    // Если ключ моложе предка, то в добавляем в левое поддерево
    else if (strcmp(key, tree->key) < 0)
        tree->left = add_balanced(tree->left, key, height, comp);
    // Если ключ старше предка, то в правое поддерево
    else if (strcmp(key, tree->key) > 0)
        tree->right = add_balanced(tree->right, key, height, comp);
    fix_height(tree);
    // Балансировка дерева
    if (balance_factor(tree) > 1 || balance_factor(tree) < -1)
        return root_balance(tree);
    else
        return tree;
}

// Проверка дерева на критерий сбалансированного АВЛ
tree_node_t *root_balance(tree_node_t *root)
{
    // Дерево не сбалансировано, высота правого поддерева на 2 больше чем левого
    if (balance_factor(root) == 2)
    {   
        // Вращаем вправо правое поддерево, если оно не сбалансировано
        if (balance_factor(root->right) < 0)
            root->right = rotate_right(root->right);
        // Для балансировки вращаем дерево влево
        return rotate_left(root);
    }
    // Высота левого поддерева на 2 больше чем правого поддерева
    if (balance_factor(root) == -2)
    {
        // Вращаем влево левое поддерево, если оно не сбалансировано
        if (balance_factor(root->left) > 0)
            root->left = rotate_left(root->left);
        // Для балансировки вращаем дерево вправо
        return rotate_right(root);
    }
    return root;
}

// Заполнение АВЛ дерева
int fill_balanced(tree_node_t **tree, buff_t buff, int64_t *time)
{
    *time = 0;
    int64_t start;
    for (size_t i = 0; i < buff.size; i++)
    {
        long height = -1;

        long comp = 0;

        start = proc_tick();
        // Добавляем элементы с балансировкой
        *tree = add_balanced(*tree, buff.buff[i], &height, &comp);
        *time += proc_tick() - start;
    }

    return EXIT_SUCCESS;
}

// Подсчёт количества элементов в АВЛ
void count_tree_nodes(tree_node_t *node, int *count)
{
    if (node)
    {
        (*count)++;
        count_tree_nodes(node->left, count);
        count_tree_nodes(node->right, count);
    }
}

// Вывод АВЛ на экран
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
        if (is_root)
            printf("%.*s", (INDENT - n) * 3, "―――――");
        printf(" %s ", node->key);
        is_root = 1;

        printf("\n");

        print_tree(node->right, tab + 1, 0, 1, (is_right ? (notch << 1) : (notch << 1) | 1), is_root);
    }
}

// =====================================================================================================

// Подсчёт количества элементов в ДДП
void count_tree_nodes_uns(tree_uns_node_t *node, int *count)
{
    if (node)
    {
        (*count)++;
        count_tree_nodes_uns(node->left, count);
        count_tree_nodes_uns(node->right, count);
    }
}

// Инициализация ДДП
int tree_node_uns_init(tree_uns_node_t **node, char *string)
{
    // Выделение памяти под корень
    *node = malloc(sizeof(tree_uns_node_t));
    // Ошибка выделения памяти
    if (!*node)
        return ALLOCATION_FAILURE;
    // Выделяем память под данные (ключ)
    (*node)->key = malloc((strlen(string) + 1) * sizeof(char));
    // Ошибка выделения памяти под данные
    if (!(*node)->key)
        return ALLOCATION_FAILURE;
    // Копируем в ключ строку
    strcpy((*node)->key, string);

    // Указатели на потомков
    (*node)->left = NULL;
    (*node)->right = NULL;

    return EXIT_SUCCESS;
}

// Добавление узла в ДДП
tree_uns_node_t  *insert_uns(tree_uns_node_t *node, char *key, long *comp)
{
    // Увеличение параметров высоты дерева и количества сравнений
    (*comp)++;
    
    // Вставка узла в дерево
    if (!node)
        tree_node_uns_init(&node, key);
    // Если младше чем предок, то влево
    else if (strcmp(key, node->key) < 0)
        node->left = insert_uns(node->left, key, comp);
    // Если старше чем предок, то вправо
    else if (strcmp(key, node->key) > 0)
        node->right = insert_uns(node->right, key, comp);

    return node;
}

// Заполнение ДДП
int fill_uns_tree(tree_uns_node_t **node, buff_t buff, int64_t *time)
{
    *time = 0;
    int64_t start;
    for (size_t i = 0; i < buff.size; i++)
    {
        long comp = 0;
        start = proc_tick();
        *node = insert_uns(*node, buff.buff[i], &comp);
        *time += proc_tick() - start;
    }
    return EXIT_SUCCESS;
}

// Вывод ДДП на экран
void print_tree_uns(tree_uns_node_t *node, size_t tab, int is_left, int is_right, unsigned int notch, int is_root)
{
    if (node)
    {
        print_tree_uns(node->left, tab + 1, 1, 0, (is_left ? (notch << 1) : (notch << 1) | 1), 0);

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
        if (is_root)
            printf("%.*s", (INDENT - n) * 3, "―――――");
        printf(" %s ", node->key);
        is_root = 1;

        printf("\n");

        print_tree_uns(node->right, tab + 1, 0, 1, (is_right ? (notch << 1) : (notch << 1) | 1), is_root);
    }
}
