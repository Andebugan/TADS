#ifndef _IN_OUT_H_
#define _IN_OUT_H_

#include <string.h>
#include <inttypes.h>
#include <stdio.h>

#define DEFAULT 20

typedef char buff_string[251];

struct buff{
    size_t size;
    buff_string buff[250];
};
typedef struct buff buff_t;

int fill_buff(const char *fname, buff_t *buff);
int file_insert(const char *fname, char *string, int64_t *time);
void main_menu();
void get_fname();
void tree_menu();
void hash_menu();
void clean_input_stream();
void greets();

#endif // _IN_OUT_H_