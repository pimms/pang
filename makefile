CC=gcc
LIBS=
INCS=
FLG=
OUTPUT=pang

SRC=$(shell find . | grep -v "\.git" | grep "\.c")
OBJ=$(SRC:.c=.o)

all:pang

pang: $(OBJ)
	@echo "LINK $(OUTPUT)"
	@$(CC) $(FLG) -o $(OUTPUT) $(OBJ) $(INCS) $(LIBS)

install: pang
	@echo "TODO!"

%.o: %.c
	@echo "CC   $<"
	@$(CC) $(FLG) -o $@ -c $< $(INCS) $(LIBS)

clean:
	@rm -f $(OBJ) $(OUTPUT)
