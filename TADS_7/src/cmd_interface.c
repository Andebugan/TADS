#include "../inc/cmd_interface.h"

void hdr(void)
{
    puts("   __                                             _     ");
    puts("  / _|_ __ ___  _ __ ___     __ _ _ __ __ _ _ __ | |__  ");
    puts(" | |_| '__/ _ \\| '_ ` _ \\   / _` | '__/ _` | '_ \\| '_ \\ ");
    puts(" |  _| | | (_) | | | | | | | (_| | | | (_| | |_) | | | |");
    puts(" |_| |_|_ \\___/|_| |_| |_|  \\__, |_|  \\__,_| .__/|_| |_|");
    puts("  \\ \\  | |_ ___   | |_ _ __ |___/___       |_|          ");
    puts("   \\ \\ | __/ _ \\  | __| '__/ _ \\/ _ \\                   ");
    puts("   / / | || (_) | | |_| | |  __/  __/                   ");
    puts("  /_/   \\__\\___/   \\__|_|  \\___|\\___|                   ");
    puts("                                                        ");
    puts("--------------------------------------------------------");
    puts("  Я В Л Я Е Т С Я   Л И   Г Р А Ф   Д Е Р Е В О М ? ");
    puts("--------------------------------------------------------");
}

void mainmenu(void)
{
    puts("");
    puts("[ 1 ] Вывести граф на экран");
    puts("[ 2 ] Удалить из графа выбранную вершину и посмотреть, будет ли граф деревом");
    puts("[ 3 ] Удалять из графа вершины поочередно и посмотреть, будет ли граф деревом");
    puts("      при удалении какой - либо из них");
    puts("");
    puts("Для выхода нажмите любую клавишу...");
}

void clean_input_stream(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}