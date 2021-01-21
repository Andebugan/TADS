CC:= gcc

CFLAGS := -std=gnu99 -Wall -Werror -pedantic -Wextra -Wfloat-equal -O0 -Wvla

OUTPATH := ./bin
SRCPATH := ./src
HDRPATH := ./inc

DFLAGS := -fPIC -std=c99
TARGET := app.exe
INC_DIR := ./inc

SRC_COMMON := $(SRCPATH)/linked_list.c $(SRCPATH)/hash_table.c $(SRCPATH)/in_out.c\
$(SRCPATH)/timer.c $(SRCPATH)/tree_funcs.c $(SRCPATH)/utils.c
SRCS := $(SRC_COMMON) $(SRCPATH)/main.c

app.exe: $(SRCS) 
	$(CC) $(CFLAGS) -o $@ $^ -I $(INC_DIR)

.PHONY : clean
clean :
	rm $(OUTPATH)**
	rm *.exe
	rm *.stackdump