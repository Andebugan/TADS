# Компилятор
CC := gcc

# Опции компиляции
CFLAGS := -std=c99 -Wall -Werror -pedantic -Wextra -Wvla
DFLAGS := -fPIC -std=c99
TARGET := app.exe
INC_DIR := ./inc

SRC_COMMON := src/stack_arr.c src/stack_list.c src/in_out.c src/process.c
SRCS := $(SRC_COMMON) src/main.c

app.exe: $(SRCS) 
	$(CC) $(DFLAGS) -o $@ $^ -I $(INC_DIR)

debug.exe: $(SRCS)
	$(CC)  $(DFLAGS) -o $@ $^ -I $(INC_DIR) -g -ggdb
