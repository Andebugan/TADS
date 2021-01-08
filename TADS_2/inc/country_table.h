#ifndef _COUNTRY_TABLE_H_
#define _COUNTRY_TABLE_H_

#include "country.h"

#define MAX_COUNTRY_TABLE_SIZE 128

extern country_t country_table[MAX_COUNTRY_TABLE_SIZE];
extern unsigned int country_table_size;

int ctrt_load_from_file(const char *filename);


#endif // _COUNTRY_TABLE_H_
