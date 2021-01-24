CC := gcc

CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic -Iinc

SRC := $(shell find ./src -iname "*.c")
OBJ := $(subst src,obj,$(subst .c,.o,$(SRC)))

TEST_SRC := $(shell find unit -iname "*.c")
TEST_OBJ := $(subst unit,test_obj,$(subst .c,.o,$(TEST_SRC)))

.PHONY : clean

clean :
	rm -rf obj test_obj *.exe

run : app.exe
	./app.exe

unit : CFLAGS += --coverage
unit : LFLAGS += --coverage
unit : unit.exe
	./unit.exe; \
	gcov -f -a -s src -o obj ./src/*.c; \
	mkdir -p ./gcov; mv *.gcov ./gcov; \
	rm -rf *.gcda *.gcno

app.exe : $(OBJ) main.c
	$(CC) $(CFLAGS) -o $@ $^

unit.exe : $(OBJ) $(TEST_OBJ) unit.c
	$(CC) $(CFLAGS) --coverage -o $@ $^

$(OBJ) : obj/%.o : src/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^

$(TEST_OBJ) : test_obj/%.o : unit/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^
