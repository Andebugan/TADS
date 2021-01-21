CC:= gcc

CFLAGS := -std=gnu99 -Wall -Werror -pedantic -Wextra -Wfloat-equal -O0

OUTPATH := ./bin/
SRCPATH := ./src/
HDRPATH := ./include/

CFLAGS += -I $(HDRPATH)

debug : CFLAGS += -g3
debug : app.exe
debug : CFLAGS += -Wno-unused-variable
debug : CFLAGS += -Wno-unused-but-set-variable

release : CFLAGS += -DNDEBUG -g0
release : app.exe

app.exe : dirs $(OUTPATH)main.o $(OUTPATH)graph.o $(OUTPATH)matrix.o $(OUTPATH)cmd_interface.o
	$(CC) $(CFLAGS) -o $@ $(OUTPATH)main.o $(OUTPATH)graph.o $(OUTPATH)matrix.o $(OUTPATH)cmd_interface.o


$(OUTPATH)%.o : $(SRCPATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

dirs: $(OUTPATH) $(SRCPATH) $(HDRPATH)

$(HDRPATH):
	mkdir $@

$(OUTPATH):
	mkdir $@

$(SRCPATH):
	mkdir $@

.PHONY : clean
clean :
	rm $(OUTPATH)**
	rm *.exe
	rm *.stackdump