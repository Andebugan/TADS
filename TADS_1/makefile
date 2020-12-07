# Компилятор
CC := gcc

# Опции компиляции
CFLAGS := -std=c99 -Wall -Werror -pedantic -Wextra -Wvla
DFLAGS := -std=c99
TARGET := app.exe
INC_DIR := ./inc

SRC_COMMON := src/num_funcs.c
SRC_IN_OUT := src/in_out.c
SRCS := $(SRC_COMMON) $(SRC_IN_OUT) src/main.c

app.exe: $(SRCS)
	$(CC) -o $(TARGET) $(DFLAGS) $(SRCS) -I $(INC_DIR)

debug.exe: $(SRCS)
	$(CC) -o debug.exe $(CFLAGS) $(SRCS) -I $(INC_DIR) -g -ggdb
