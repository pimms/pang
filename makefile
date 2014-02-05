CC=gcc
LIBS=-Lpangvm -lpangvm
INCS=
FLG=
OUTPUT=pang

SRC=$(shell find . | grep -v "^\.git/\|^\./pangvm/" | grep "\.c")
OBJ=$(SRC:.c=.o)

all:pang

pang:pangVM $(OBJ)
	@echo "LINK $(OUTPUT)"
	@$(CC) $(FLG) -o $(OUTPUT) $(OBJ) $(INCS) $(LIBS)

pangVM:
	@cd pangvm && make

%.o: %.c
	@echo "CC   $<"
	@$(CC) $(FLG) -o $@ -c $< $(INCS) $(LIBS)

clean:
	@rm -f $(OBJ) $(OUTPUT)
	@cd pangvm && make clean
