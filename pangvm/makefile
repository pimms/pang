INC =
LIBS=
FLG =

CC = gcc

TARGET_NAME = libpangvm.a

SRC = $(shell find -name '*.c' -print)
OBJ = $(subst .c,.o,$(SRC))

all: $(OBJ)
	@echo "LINK $(TARGET_NAME)"
	@ar rvs $(TARGET_NAME) $(OBJ) >/dev/null

%.o: %.c
	@echo "CC   $<"
	@$(CC) $(FLG) -o $@ -c $< $(INC) $(LIBS)

clean:
	@rm -rf $(OBJ) $(TARGET_NAME)
