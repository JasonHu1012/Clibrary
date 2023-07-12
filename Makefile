INCLUDE = include/
SRC = src/
TEST = test/
OBJ = obj/
LIB = lib/
BIN = bin/

CC = gcc
CFLAGS = -Wall -I$(INCLUDE)

AR = ar
ARFLAGS = rcs

LIBRARYS = list stack

all: $(addsuffix .a, $(addprefix $(LIB)lib, $(LIBRARYS))) $(addsuffix _test, $(addprefix $(BIN), $(LIBRARYS)))

$(BIN)%_test: $(TEST)%_test.c $(LIB)lib%.a | $(BIN)
	$(CC) -o $@ $(CFLAGS) $^

$(LIB)liblist.a: $(OBJ)list.o | $(LIB)
	$(AR) $(ARFLAGS) $@ $^

$(LIB)libstack.a: $(OBJ)stack.o $(OBJ)list.o | $(LIB)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ)%.o: $(SRC)%.c $(INCLUDE)%.h | $(OBJ)
	$(CC) -o $@ -c $(CFLAGS) $<

.PHONY:

clean:
	rm -rf $(OBJ) $(LIB) $(BIN)

$(OBJ):
	mkdir -p $(OBJ)

$(LIB):
	mkdir -p $(LIB)

$(BIN):
	mkdir -p $(BIN)
