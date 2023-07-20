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

LIBRARYS = list stack deque ndarray vector

.PHONY: all test %.test clean

all: $(addsuffix .a, $(addprefix $(LIB)lib, $(LIBRARYS))) $(addsuffix _test, $(addprefix $(BIN), $(LIBRARYS)))

test: $(addsuffix .test,  $(LIBRARYS))

%.test: $(BIN)%_test
	$<

$(BIN)list_test: $(TEST)list_test.c $(LIB)liblist.a | $(BIN)
	$(CC) -o $@ $(CFLAGS) $^

$(BIN)stack_test: $(TEST)stack_test.c $(LIB)libstack.a | $(BIN)
	$(CC) -o $@ $(CFLAGS) $^

$(BIN)deque_test: $(TEST)deque_test.c $(LIB)libdeque.a | $(BIN)
	$(CC) -o $@ $(CFLAGS) $^

$(BIN)ndarray_test: $(TEST)ndarray_test.c $(LIB)libndarray.a | $(BIN)
	$(CC) -o $@ $(CFLAGS) $^

$(BIN)vector_test: $(TEST)vector_test.c $(LIB)libvector.a | $(BIN)
	$(CC) -o $@ $(CFLAGS) $^ -lm

$(LIB)liblist.a: $(OBJ)list.o | $(LIB)
	$(AR) $(ARFLAGS) $@ $^

$(LIB)libstack.a: $(OBJ)stack.o $(OBJ)list.o | $(LIB)
	$(AR) $(ARFLAGS) $@ $^

$(LIB)libdeque.a: $(OBJ)deque.o | $(LIB)
	$(AR) $(ARFLAGS) $@ $^

$(LIB)libndarray.a: $(OBJ)ndarray.o | $(LIB)
	$(AR) $(ARFLAGS) $@ $^

$(LIB)libvector.a: $(OBJ)vector.o | $(LIB)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ)%.o: $(SRC)%.c $(INCLUDE)%.h | $(OBJ)
	$(CC) -o $@ -c $(CFLAGS) $<

clean:
	rm -rf $(OBJ) $(LIB) $(BIN)

$(OBJ):
	mkdir -p $(OBJ)

$(LIB):
	mkdir -p $(LIB)

$(BIN):
	mkdir -p $(BIN)
