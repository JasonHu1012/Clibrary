CC=gcc
INCLUDE=include
SRC=src
LIB=lib

sample: sample.c $(LIB)/liblist.a $(LIB)/libmatrix.a $(LIB)/libqueue.a $(LIB)/libstack.a $(LIB)/libvector.a $(LIB)/libndarray.a $(LIB)/libfunc.a $(LIB)/libcollision.a
	$(CC) sample.c -o sample -I$(INCLUDE) -L$(LIB) -llist -lmatrix -lqueue -lstack -lvector -lm -lndarray -lfunc -lcollision $(shell sdl2-config --cflags --libs)

$(LIB)/liblist.a: $(LIB)/list.o $(LIB)/lnode.o
	ar rcs $(LIB)/liblist.a $(LIB)/list.o $(LIB)/lnode.o
$(LIB)/libmatrix.a: $(LIB)/matrix.o
	ar rcs $(LIB)/libmatrix.a $(LIB)/matrix.o
$(LIB)/libqueue.a: $(LIB)/queue.o $(LIB)/lnode.o
	ar rcs $(LIB)/libqueue.a $(LIB)/queue.o $(LIB)/lnode.o
$(LIB)/libstack.a: $(LIB)/stack.o $(LIB)/lnode.o
	ar rcs $(LIB)/libstack.a $(LIB)/stack.o $(LIB)/lnode.o
$(LIB)/libvector.a: $(LIB)/vector.o
	ar rcs $(LIB)/libvector.a $(LIB)/vector.o
$(LIB)/libndarray.a: $(LIB)/ndarray.o
	ar rcs $(LIB)/libndarray.a $(LIB)/ndarray.o
$(LIB)/libfunc.a: $(LIB)/func.o
	ar rcs $(LIB)/libfunc.a $(LIB)/func.o
$(LIB)/libcollision.a: $(LIB)/collision.o $(LIB)/list.o $(LIB)/lnode.o $(LIB)/vector.o $(LIB)/ndarray.o
	ar rcs $(LIB)/libcollision.a $(LIB)/collision.o $(LIB)/list.o $(LIB)/lnode.o $(LIB)/vector.o $(LIB)/ndarray.o

$(LIB)/list.o: $(SRC)/list.c $(INCLUDE)/list.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/list.c -o $(LIB)/list.o -I$(INCLUDE)
$(LIB)/lnode.o: $(SRC)/lnode.c $(INCLUDE)/lnode.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/lnode.c -o $(LIB)/lnode.o -I$(INCLUDE)
$(LIB)/matrix.o: $(SRC)/matrix.c $(INCLUDE)/matrix.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/matrix.c -o $(LIB)/matrix.o -I$(INCLUDE)
$(LIB)/queue.o: $(SRC)/queue.c $(INCLUDE)/queue.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/queue.c -o $(LIB)/queue.o -I$(INCLUDE)
$(LIB)/stack.o: $(SRC)/stack.c $(INCLUDE)/stack.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/stack.c -o $(LIB)/stack.o -I$(INCLUDE)
$(LIB)/vector.o: $(SRC)/vector.c $(INCLUDE)/vector.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/vector.c -o $(LIB)/vector.o -I$(INCLUDE)
$(LIB)/ndarray.o: $(SRC)/ndarray.c $(INCLUDE)/ndarray.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/ndarray.c -o $(LIB)/ndarray.o -I$(INCLUDE)
$(LIB)/func.o: $(SRC)/func.c $(INCLUDE)/func.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/func.c -o $(LIB)/func.o -I$(INCLUDE)
$(LIB)/collision.o: $(SRC)/collision.c $(INCLUDE)/collision.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/collision.c -o $(LIB)/collision.o -I$(INCLUDE)

clean:
	rm -rf lib sample
run: sample
	./sample
