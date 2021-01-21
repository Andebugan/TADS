CC := gcc

CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic -Iinc

SRC := $(shell find src -name *.c)
OBJ := $(subst src,obj,$(subst .c,.o,$(SRC)))

run : app.exe
	./app.exe

app.exe : $(OBJ)
	$(CC) -o $@ $^

$(OBJ) : obj/%.o : src/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^
