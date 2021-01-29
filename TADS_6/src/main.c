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
    tree_uns_node_t *tree_uns_root = NULL;
    // Переменная для хранения сбалансированного ДДП
    tree_node_t *tree_root = NULL;
    // Указатель на хеш-таблицу
    hash_linked_t *hash_table = NULL;
    // Размер хеш-таблицы
    size_t hash_size;
    // Среднее значение коллизий
    size_t avg_collisions;
    // Переменная для определения перестроенной/неперестроенной таблицы
    int func = 0;
    // Переменная для определения ДДП или нет
    int tree_uns_built = 0;
    // Переменная для определения АВЛ или нет
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
        clean_input_stream();
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
                    // Выбор режима работы с ДДП
                    int tree_choice;
                    // Переменная для замера времени
                    int64_t time;
                    // Меню работы с деревом поиска
                    tree_menu();
                    // Работа с ДДП
                    while (scanf("%d", &tree_choice) == 1 && tree_choice != 0)
                    {
                        // Постоить ДДП и вывести на экран
                        if (tree_choice == 1)
                        {
                            // Существует построенное дерево ДДП
                            tree_uns_built = 1;
                            // Заполняем ДДП
                            fill_uns_tree(&tree_uns_root, buff, &time);
                            // Выводим несбалансированное дерево
                            print_tree_uns(tree_uns_root, 0, 0, 0, 0U, 0);
                            clean_input_stream();
                        }
                        // Построить АВЛ и вывести на экран
                        else if (tree_choice == 2)
                        {
                            // Существует построенное дерево АВЛ
                            tree_built = 1;
                            // Заполняем ДДП
                            fill_balanced(&tree_root, buff, &time);
                            // Выводим несбалансированное дерево
                            print_tree(tree_root, 0, 0, 0, 0U, 0);
                            clean_input_stream();
                        }
                        // Добавить слово в несбалансированное ДДП
                        else if (tree_choice == 3)
                        {
                            if (!tree_uns_built)
                            {
                                puts(RED "Ошибка! Сначала дерево нужно построить." RESET);
                                clean_input_stream();
                                tree_menu();
                            }
                            else
                            {
                                long comp = 0;
                                printf(CYAN "Введите добавляемое слово: " RESET);
                                char word[255];
                                scanf("%s", word);
                                // Добавляем в несбалансированное дерево
                                add_unbalanced(&tree_uns_root, word, &comp);

                                print_tree_uns(tree_uns_root, 0, 0, 0, 0U, 0);
                                clean_input_stream();
                            }
                        }
                        // Добавить слово в АВЛ
                        else if (tree_choice == 4)
                        {
                            if (!tree_built)
                            {
                                puts(RED "Ошибка! Сначала дерево нужно построить." RESET);
                                clean_input_stream();
                                tree_menu();
                            }
                            else
                            {
                                long comp = 0;
                                printf(CYAN "Введите добавляемое слово: " RESET);
                                char word[255];
                                scanf("%s", word); 
                                // Добавляем в АВЛ
                                add_elem_balanced(&tree_root, word, &comp);

                                print_tree(tree_root, 0, 0, 0, 0U, 0);
                                clean_input_stream();
                            }
                        }
                        // Вывод АВЛ
                        else if (tree_choice == 5)
                        {
                            if (!tree_built)
                            {
                                puts(RED "Ошибка! Сначала АВЛ нужно построить." RESET);
                                clean_input_stream();
                                tree_menu();
                            }
                            else
                            {
                                print_tree(tree_root, 0, 0, 0, 0U, 0);
                                clean_input_stream();
                            }
                        }
                        // Вывод ДДП
                        else if (tree_choice == 6)
                        {
                            if (!tree_uns_built)
                            {
                                puts(RED "Ошибка! Сначала ДДП нужно построить." RESET);
                                clean_input_stream();
                                tree_menu();
                            }
                            else
                            {
                                print_tree_uns(tree_uns_root, 0, 0, 0, 0U, 0);
                                clean_input_stream();
                            }
                        }
                        // Сравнить время добавления в сбалансированное и несбалансированное ДДП
                        else if (tree_choice == 5)
                        {
                            long comp = 0;
                            int64_t balance_time = 0, not_balance_time = 0;

                            printf(CYAN "Введите добавляемое слово: " RESET);
                            char word[255];

                            scanf("%s", word);
                            
                            // Добавляем с подсчётом времени
                            balance_time = add_elem_balanced(&tree_root, word, &comp);
                            not_balance_time = add_unbalanced(&tree_uns_root, word, &comp);

                            puts(YELLOW "Полученные замеры времени: " RESET);
                            printf(CYAN "АВЛ: " RESET " %.3lf\n", (float)balance_time / buff.size);
                            printf(CYAN "ДДП: " RESET " %.3lf\n\n", (float)not_balance_time / buff.size);

                            printf(YELLOW "АВЛ:\n" RESET);
                            print_tree(tree_root, 0, 0, 0, 0U, 0);
                            printf(YELLOW "ДДП:\n" RESET);
                            print_tree_uns(tree_uns_root, 0, 0, 0, 0U, 0);

                            clean_input_stream();
                        }
                        else if (tree_choice == 0)
                        {
                            main_menu();
                            clean_input_stream();
                            break;
                        }
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
        else if (choice == 2)
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
                    while (scanf("%d", &hash_choice) == 1 && hash_choice != 0)
                    {
                        // Создание таблицы на основе данных из файла
                        if (hash_choice == 1)
                        {
                            puts(CYAN "Введите максимально допустимое количество коллизий для одного ключа: " RESET);
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
                                        // Заполнение таблицы
                                        collisions = hash_table_fill(&hash_table, hash_size, &buff, key_div);
                                        if (collisions > avg_collisions)
                                        {
                                            // Пересоздание таблицы в случае превышения максимального количества коллизий для одного ключа
                                            puts(YELLOW "Превышено максимальное число коллизий для одного ключа. Происходит перестройка таблицы ..." RESET);
                                            free_table(hash_table, hash_size);
                                            hash_table = calloc(hash_size, sizeof(hash_linked_t));
                                            if (!hash_table)
                                            {
                                                puts(RED "Похоже, произошла системная проблема. Попробуйте еще раз." RESET);
                                                hash_menu();
                                                clean_input_stream();
                                                continue;
                                            }
                                            collisions = hash_table_fill(&hash_table, hash_size, &buff, bit_hash);
                                            func = 1;
                                        }
                                        else
                                            func = 0;
                                        puts(GREEN "Успешно." RESET);
                                    }
                                }
                            }
                        }
                        // Вывод таблицы на экран
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
                        // Добавление элемента в таблицу
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

                            // Создание ключа для слова в зависимости от того, перестроенная таблица, или нет
                            if (func == 0)
                                key = key_div(word, hash_size);
                            else
                                key = bit_hash(word, hash_size);

                            list_insert(&hash_table[key], word, &hash_time);

                            puts(YELLOW "Полученные замеры:" RESET);
                            printf(CYAN "Время добавления в хэш-таблицу: " RESET "%ld\n", (long)hash_time);
                            size_t data_size = 0;
                            for (size_t i = 0; i < buff.size; i++)
                                data_size += strlen(buff.buff[i]);
                        }
                        else if (hash_choice == 0)
                        {
                            main_menu();
                            clean_input_stream();
                            break;
                        }
                        hash_menu();
                    }
                    clean_input_stream();
                }
            }
        }
        // Сравнения добавления элемента в разные структуры данных
        else if (choice == 3)
        {
            puts(YELLOW "Введите имя файла: " RESET);
            int rc = 0;
            long comp_avl = 0;
            long comp_ddp = 0;
            long comp_hash = 0;
            scanf("%s", fname);
            int64_t time_avl;
            int64_t time_hash;
            int64_t time_ddp;
            int64_t time_file;
            size_t key;
            printf(YELLOW "Введите добавляемое слово: " RESET);
            char word[255];
            scanf("%s", word);
            rc = file_insert(fname, word, &time_file);
            if (rc == INVALID_FNAME)
                puts(RED "Файл не найден. Попробуйте снова." RESET);
            if (rc == 0 && tree_root == NULL)
            {
                rc = -1;
                puts(RED "АВЛ не создано. Сначала создайте АВЛ в меню работы с бинарным деревом." RESET);
            }
            else if (rc == 0)
                time_avl = add_elem_balanced(&tree_root, word, &comp_avl);
            if (rc == 0 && tree_uns_root == NULL)
            {
                rc = -1;
                puts(RED "ДДП не создано. Сначала создайте ДДП в меню работы с бинарным деревом." RESET);
            }
            else if (rc == 0)
                time_ddp = add_unbalanced(&tree_uns_root, word, &comp_ddp);
            if (rc == 0 && hash_table == NULL)
            {
                rc = -1;
                puts(RED "Хеш-таблица не построена. Сначала создайте таблицу в меню работы с хеш-таблицей." RESET);
            }
            else if (rc == 0)
            {
                if (func == 0)
                    key = key_div(word, hash_size);
                else
                    key = bit_hash(word, hash_size);
                comp_hash = list_insert(&hash_table[key], word, &time_hash);
                printf(CYAN "Полученные замеры:\n" RESET);
                printf("Время добавления в файл: %ld\n", (long)time_file);
                printf("Время добавления в АВЛ: %ld\n", (long)time_avl);
                printf("Время добавления в ДДП: %ld\n", (long)time_ddp);
                printf("Время добавления в хеш-таблицу: %ld\n", (long)time_hash);
                printf("Количество сравнений АВЛ: %ld\n", comp_avl);
                printf("Количество сравнений ДДП: %ld\n", comp_ddp);
                printf("Количество сравнений хеш-таблицы: %ld\n", comp_hash);

                int count = 0;
                count_tree_nodes(tree_root, &count);
                printf("Память АВЛ: %ld\n", count * sizeof(tree_node_t));
                count = 0;
                count_tree_nodes_uns(tree_uns_root, &count);
                printf("Память ДДП: %ld\n", count * sizeof(tree_uns_node_t));
                printf("Память хеш-таблицы: %ld\n", table_elem_col(hash_table, hash_size));
                
                printf("Хеш-таблица:\n");
                hash_table_print(hash_table, hash_size);
                printf("ДДП:\n");
                print_tree_uns(tree_uns_root, 0, 0, 0, 0U, 0);
                printf("АВЛ:\n");
                print_tree(tree_root, 0, 0, 0, 0U, 0);
            }
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