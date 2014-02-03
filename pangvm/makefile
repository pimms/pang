INC =
LIBS=
FLG = -g -std=c99

CC = gcc

LIBOUT=libpangvm.a
OUT = pangvm

SRC = $(shell find -name '*.c' -print | grep -v "main.c")
OBJ = $(subst .c,.o,$(SRC))

all:$(LIBOUT) $(OUT)

$(LIBOUT): $(OBJ)
	@echo "LINK $(LIBOUT)"
	@ar rvs $(LIBOUT) $(OBJ) >/dev/null

$(OUT): $(OBJ) main.o
	@echo "LINK $(OUT)"
	@$(CC) $(FLG) -o $(OUT) $(OBJ) main.o $(INC) $(LIBS)

%.o: %.c
	@echo "CC   $<"
	@$(CC) $(FLG) -o $@ -c $< $(INC) $(LIBS)

clean:
	@rm -rf $(OBJ) $(TARGET_NAME)
