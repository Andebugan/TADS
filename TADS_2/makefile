CC := gcc

CFLAGS := -std=c99 -Wall -Werror -pedantic -Wextra -Iinc

SRC := $(shell find src -name *.c)

OBJ := $(subst .c,.o,$(SRC))
OBJ := $(subst src,obj,$(OBJ))

run : app.exe
	./app.exe

app.exe : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ) : obj/%.o : src/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^
