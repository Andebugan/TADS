#define __USE_MINGW_ANSI_STDIO 1
#include "../inc/graph.h"
#include "../inc/cmd_interface.h"

int main(void)
{
    setbuf(stdout, NULL);
    hdr();

    matrix_t graph;
    char filename[257];
    int choice;
    size_t rows = 0;
    printf("Введите количество вершин графа: ");
    int error = scanf("%zu", &rows) != 1 && rows > 100;
    while (error)
    {
        puts("Ввод неверный. Попробуйте еще раз.\n");
        clean_input_stream();
        printf("Введите количество вершин графа: ");
        error = scanf("%zu", &rows) != 1 || rows > 100;
    }
    printf("Введите имя файла, из которого будут импортироваться данные : ");
    scanf("%s", filename);
    if (allocate(&graph, rows, rows) == ALLOCATION_FAILURE)
    {
        puts("Похоже, произошла ситемная ошибка. Попробуйте еще раз.");
        return EXIT_FAILURE;
    }
    zerofy(&graph);

    error = make_adjacency(&graph, filename);
    if (error)
    {
        if (error == ALLOCATION_FAILURE)
        {
            puts("Похоже, произошла ситемная ошибка. Попробуйте еще раз.");
            return EXIT_FAILURE;
        }
        if (error == EXIT_FAILURE)
        {
            puts("Файл заполнен некорректно. Проверьте содержание файла.");
            return EXIT_FAILURE;
        }
        if (error == INX_DONT_MATCH)
        {
            puts("Вы указали слишком малое количество вершин графа.");
            return EXIT_FAILURE;
        }
    }

    size_t to_delete = 0;
    int yes;
    mainmenu();
    while (scanf("%d", &choice) == 1)
    {
        if (choice == 1)
        {
            char filename[250] = "graph_dot.txt";
            export_to_dot(filename, graph, -1);
            system("dot -Tpng graph_dot.txt -o graph.png");
            puts("\nСмотрите файл graph.png в директории приложения.\n");
        }
        if (choice == 2)
        {
            printf("Введите номер вершины, которую нужно удалить :");
            if (scanf("%zu", &to_delete) != 1 || to_delete >= rows)
                puts("Номер вершины введен неверно.");
            else
            {
                matrix_t graph_copy;
                if (allocate(&graph_copy, rows, rows) == ALLOCATION_FAILURE)
                {
                    puts("Похоже, произошла ситемная ошибка. Попробуйте еще раз.");
                    return EXIT_FAILURE;
                }
                copy(graph, &graph_copy);
                remove_edge(&graph_copy, to_delete);
                if (is_tree(graph_copy))
                    printf("При удалении вершины %zu получается дерево.\nВы хотите вывести его в файл? (1 - да, 0 - нет) : ", to_delete);
                else
                    printf("При удалении вершины %zu дерево не получается.\nВы хотите вывести граф в файл? (1 - да, 0 - нет) : ", to_delete);
                error = scanf("%d", &yes);
                while (!error)
                {
                    clean_input_stream();
                    error = scanf("%d", &yes);
                }
                if (yes)
                {
                    char filename[250] = "deleted_edge.txt";
                    export_to_dot(filename, graph_copy, to_delete);
                    system("dot -Tpng deleted_edge.txt -o graph.png");
                }
                free_matrix(&graph_copy);
            }
        }
        else if (choice == 3)
        {

            int found = 0;
            for (to_delete = 0; to_delete < rows; to_delete++)
            {
                matrix_t graph_copy;
                if (allocate(&graph_copy, rows, rows) == ALLOCATION_FAILURE)
                {
                    puts("Похоже, произошла ситемная ошибка. Попробуйте еще раз.");
                    return EXIT_FAILURE;
                }
                copy(graph, &graph_copy);
                remove_edge(&graph_copy, to_delete);
                if (is_tree(graph_copy))
                {
                    found = 1;
                    printf("При удалении вершины %zu получается дерево.\nВы хотите вывести его в файл? (1 - да, 0 - нет) :", to_delete);
                    error = scanf("%d", &yes);
                    while (!error)
                    {
                        clean_input_stream();
                        error = scanf("%d", &yes);
                    }
                    if (yes)
                    {
                        char filename[250] = "deleted_edge.txt";
                        export_to_dot(filename, graph_copy, to_delete);
                        system("dot -Tpng deleted_edge.txt -o graph.png");
                    }
                }
                free_matrix(&graph_copy);
            }
            if (!found)
            {
                puts("Похоже, при удалении любой вершины, дерева не получится.");
            }
        }
        mainmenu();
    }
    free_matrix(&graph);
    return EXIT_SUCCESS;
}