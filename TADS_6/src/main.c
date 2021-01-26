#define __USE_MINGW_ANSI_STDIO 1
#define MAX_SIZE 10000
#include "colors.h"
#include "utils.h"

// Основная функция программы
int main(void)
{
    // Имя файла
    char fname[250];
    // Переменная выбора режима работы с программой
    int choice;
    // Переменная для хранения кода ошибок
    int rc;
    // Переменная-буффер для хранения данных из файла
    buff_t buff;
    // Переменная для хранения корня несбалансированного дерева
    tree_node_t *tree_root;
    // Переменная для хранения сбалансированного ДДП
    tree_node_t *tree_root_balanced;
    // Указатель на хеш-таблицу
    hash_linked_t *hash_table;
    // Размер хеш-таблицы
    size_t hash_size;
    // Среднее значение коллизий
    size_t avg_collisions;
    // Переменная для определения перестроенной/неперестроенной таблицы
    int func = 0;
    // Переменная для определения построено дерево или нет
    int tree_built = 0;
    // Переменная для определения построена ли хеш-таблица или нет
    int hash_built = 0;

    // Очистка потоков ввода/вывода
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    // Вывод приветсвтенного сообщения
    greets();
    // Вывод главного меню
    main_menu();
    // Обработка ввода
    while (scanf("%d", &choice) == 1)
    {
        // В случае обработки дерева поиска
        if (choice == 1)
        {
            // Ввод имени файла
            get_fname();
            scanf("%s", fname);
            {
                // Передача данных файла в буфер
                rc = fill_buff(fname, &buff);
                // Обработка ошибочных ситуаций
                if (rc == INVALID_FNAME)
                {
                    puts(RED "Файл с таким именем отсутствует, попробуйте снова." RESET);
                    clean_input_stream();
                    continue;
                }
                else if (rc == EMPTY_SOURCE)
                {
                    puts(RED "Файл не содержит никаких данных, попробуйте снова." RESET);
                    clean_input_stream();
                    continue;
                }
                else if (rc == EXIT_SUCCESS)
                {
                    int tree_choice;
                    int64_t time;
                    // Меню работы с деревом поиска
                    tree_menu();
                    // Работа с ДДП
                    while (scanf("%d", &tree_choice) == 1)
                    {
                        // Построение дерева
                        if (tree_choice == 1)
                        {
                            tree_built = 1;
                            fill_balanced(&tree_root_balanced, buff, &time);
                            fill_tree(&tree_root, buff, &time);
                            print_tree(tree_root, 0, 0, 0, 0U, 0);
                            clean_input_stream();
                        }
                        // Вывод дерева
                        else if (tree_choice == 2)
                        {
                            if (!tree_built)
                            {
                                puts(RED "Ошибка! Сначала дерево нужно построить." RESET);
                                clean_input_stream();
                                tree_menu();
                            }
                            else
                            {
                                print_tree(tree_root_balanced, 0, 0, 0, 0U, 0);
                                clean_input_stream();
                            }
                        }
                        // Добавить слово в несбалансированное ДДП
                        else if (tree_choice == 3)
                        {
                            if (!tree_built)
                            {
                                puts(RED "Ошибка! Сначала дерево нужно построить." RESET);
                                clean_input_stream();
                                tree_menu();
                            }
                            else
                            {
                                printf(CYAN "Введите добавляемое слово: " RESET);
                                char word[255];
                                scanf("%s", word);
                                add_unbalanced(&tree_root, word);
                                add_elem_balanced(&tree_root_balanced, word);

                                print_tree(tree_root, 0, 0, 0, 0U, 0);
                                clean_input_stream();
                            }
                        }
                        // Добавить слово в сбалансированное ДДП
                        else if (tree_choice == 4)
                        {
                            printf(CYAN "Введите добавляемое слово: " RESET);
                            char word[255];
                            scanf("%s", word);
                            add_elem_balanced(&tree_root_balanced, word);
                            add_unbalanced(&tree_root, word);

                            print_tree(tree_root_balanced, 0, 0, 0, 0U, 0);
                            clean_input_stream();
                        }
                        // Сравнить время добавления в сбалансированное и несбалансированное ДДП
                        else if (tree_choice == 5)
                        {
                            int64_t balance_time = 0, not_balance_time = 0;

                            tree_node_t *test_tree = NULL;
                            tree_node_t *test_tree_balanced = NULL;

                            fill_tree(&test_tree, buff, &not_balance_time);

                            fill_balanced(&test_tree_balanced, buff, &balance_time);

                            puts(YELLOW "Полученные замеры: " RESET);
                            printf(CYAN "Сбалансированное ДДП: " RESET " %.3lf\n", (float)balance_time / buff.size);
                            printf(CYAN "Несбалансированное ДДП: " RESET " %.3lf\n", (float)not_balance_time / buff.size);

                            clean_input_stream();
                        }
                        // Обработка ошибочного ввода
                        else
                        {
                            clean_input_stream();
                            break;
                        }
                        tree_menu();
                    }
                }
            }
            clean_input_stream();
        }
        // Работа с хеш-таблицей
        if (choice == 2)
        {
            // Получение имени файла
            get_fname();
            scanf("%s", fname);
            {
                // Передача данных файла в буфер
                rc = fill_buff(fname, &buff);
                if (rc == INVALID_FNAME)
                {
                    puts(RED "Файл с таким именем отсутствует, попробуйте снова." RESET);
                    clean_input_stream();
                    continue;
                }
                else if (rc == EMPTY_SOURCE)
                {
                    puts(RED "Файл не содержит никаких данных, попробуйте снова." RESET);
                    clean_input_stream();
                    continue;
                }
                // Работа с хещ-таблицей
                else if (rc == EXIT_SUCCESS)
                {
                    int hash_choice;
                    // Вывод меню
                    hash_menu();
                    while (scanf("%d", &hash_choice) == 1)
                    {
                        // Создание таблицы на основе данных из файла
                        if (hash_choice == 1)
                        {
                            puts(CYAN "Введите среднее допустимое количество коллизий: " RESET);
                            if (scanf("%zu", &avg_collisions) != 1)
                            {
                                puts(RED "Число введено неверно." RESET);
                                hash_menu();
                                clean_input_stream();
                            }
                            else if (avg_collisions == 0)
                            {
                                puts(RED "Число введено неверно." RESET);
                                hash_menu();
                                clean_input_stream();
                            }
                            else
                            {
                                puts(CYAN "Введите максимальный размер хэш-таблицы: " RESET);
                                if (scanf("%zu", &hash_size) != 1)
                                {
                                    puts(RED "Размеры введены неверно." RESET);
                                    hash_menu();
                                    clean_input_stream();
                                }
                                else if (hash_size == 0 || hash_size > MAX_SIZE)
                                {
                                    puts(RED "Размеры введены неверно." RESET);
                                    hash_menu();
                                    clean_input_stream();
                                }
                                else
                                {
                                    hash_table = calloc(hash_size, sizeof(hash_linked_t));
                                    // Проверка выделения памяти
                                    if (!hash_table)
                                    {
                                        puts(RED "Произошла системная проблема. Попробуйте снова." RESET);
                                        hash_menu();
                                        clean_input_stream();
                                    }
                                    else
                                    {
                                        // Создание таблицы с корректными параметрами
                                        hash_built = 1;
                                        size_t collisions;
                                        collisions = hash_table_fill(&hash_table, hash_size, &buff, key_div);
                                        if (collisions > avg_collisions)
                                        {
                                            // Пересоздание таблицы в случае превышения свреднего количества коллизий
                                            puts(YELLOW "Превышено максимальное число коллизий. Происходит перестройка таблицы ..." RESET);
                                            free_table(hash_table, hash_size);
                                            hash_table = calloc(hash_size, sizeof(hash_linked_t));
                                            if (!hash_table)
                                            {
                                                puts(RED "Похоже, произошла системная проблема. Попробуйте еще раз." RESET);
                                                hash_menu();
                                                clean_input_stream();
                                                continue;
                                            }
                                            collisions = hash_table_fill(&hash_table, hash_size, &buff, Jenkins_hash);
                                            func = 1;
                                        }
                                        else
                                            func = 0;
                                        puts(GREEN "Успешно." RESET);
                                    }
                                }
                            }
                        }
                        if (hash_choice == 2)
                        {
                            if (!hash_built)
                            {
                                puts(RED "Ошибка! Хеш-таблица ещё не построена." RESET);
                                hash_menu();
                                clean_input_stream();
                                continue;
                            }
                            hash_table_print(hash_table, hash_size);
                        }
                        if (hash_choice == 3)
                        {
                            if (!hash_built)
                            {
                                puts(RED "Ошибка! Хеш-таблица ещё не построена." RESET);
                                hash_menu();
                                clean_input_stream();
                                continue;
                            }
                            int64_t hash_time = 0;
                            printf(CYAN "Введите добавляемое слово: " RESET);
                            char word[255];
                            scanf("%s", word);
                            size_t key;

                            if (func == 0)
                                key = key_div(word, hash_size);
                            else
                                key = Jenkins_hash(word, hash_size);

                            list_insert(&hash_table[key], word, &hash_time);

                            puts(YELLOW "Полученные замеры:" RESET);
                            printf(CYAN "Время добавления в хэш-таблицу: " RESET "%ld\n", (long)hash_time);
                            size_t data_size = 0;
                            for (size_t i = 0; i < buff.size; i++)
                                data_size += strlen(buff.buff[i]);
                        }
                        hash_menu();
                    }
                    clean_input_stream();
                }
            }
        }
        if (choice == 3)
        {
            puts(CYAN "Введите имя файла: ");
            scanf("%s", fname);
            int64_t time;
            printf("Введите добавляемое слово: " RESET);
            char word[255];
            scanf("%s", word);
            int rc = file_insert(fname, word, &time);
            if (rc == INVALID_FNAME)
                puts(RED "Файл не найден. Попробуйте снова." RESET);
            printf(CYAN "Полученные замеры:" RESET "%ld\n", (long)time);
        }
        else
        {
            puts(RED "Команда введена некорректно, попробуйте снова." RESET);
            clean_input_stream();
        }
        main_menu();
    }
    free_table(hash_table, hash_size);
    return EXIT_SUCCESS;
}