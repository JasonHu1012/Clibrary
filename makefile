CC = gcc
INCLUDE = include
SRC = src
LIB = lib
AR = ar
CFLAGS = -I$(INCLUDE)

sample: sample.c $(LIB)/liblist.a $(LIB)/libmatrix.a $(LIB)/libqueue.a $(LIB)/libstack.a $(LIB)/libvector.a $(LIB)/libndarray.a $(LIB)/libfunc.a $(LIB)/libcollision.a
	$(CC) sample.c -o sample -L$(LIB) -llist -lmatrix -lqueue -lstack -lvector -lm -lndarray -lfunc -lcollision $(shell sdl2-config --cflags --libs) $(CFLAGS)

$(LIB)/liblist.a: $(LIB)/list.o $(LIB)/lnode.o
	$(AR) rcs $(LIB)/liblist.a $(LIB)/list.o $(LIB)/lnode.o
$(LIB)/libmatrix.a: $(LIB)/matrix.o
	$(AR) rcs $(LIB)/libmatrix.a $(LIB)/matrix.o
$(LIB)/libqueue.a: $(LIB)/queue.o $(LIB)/lnode.o
	$(AR) rcs $(LIB)/libqueue.a $(LIB)/queue.o $(LIB)/lnode.o
$(LIB)/libstack.a: $(LIB)/stack.o $(LIB)/lnode.o
	$(AR) rcs $(LIB)/libstack.a $(LIB)/stack.o $(LIB)/lnode.o
$(LIB)/libvector.a: $(LIB)/vector.o
	$(AR) rcs $(LIB)/libvector.a $(LIB)/vector.o
$(LIB)/libnd$(AR)ray.a: $(LIB)/nd$(AR)ray.o
	$(AR) rcs $(LIB)/libnd$(AR)ray.a $(LIB)/nd$(AR)ray.o
$(LIB)/libfunc.a: $(LIB)/func.o
	$(AR) rcs $(LIB)/libfunc.a $(LIB)/func.o
$(LIB)/libcollision.a: $(LIB)/collision.o $(LIB)/list.o $(LIB)/lnode.o $(LIB)/vector.o $(LIB)/nd$(AR)ray.o
	$(AR) rcs $(LIB)/libcollision.a $(LIB)/collision.o $(LIB)/list.o $(LIB)/lnode.o $(LIB)/vector.o $(LIB)/nd$(AR)ray.o

$(LIB)/list.o: $(SRC)/list.c $(INCLUDE)/list.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/list.c -o $(LIB)/list.o $(CFLAGS)
$(LIB)/lnode.o: $(SRC)/lnode.c $(INCLUDE)/lnode.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/lnode.c -o $(LIB)/lnode.o $(CFLAGS)
$(LIB)/matrix.o: $(SRC)/matrix.c $(INCLUDE)/matrix.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/matrix.c -o $(LIB)/matrix.o $(CFLAGS)
$(LIB)/queue.o: $(SRC)/queue.c $(INCLUDE)/queue.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/queue.c -o $(LIB)/queue.o $(CFLAGS)
$(LIB)/stack.o: $(SRC)/stack.c $(INCLUDE)/stack.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/stack.c -o $(LIB)/stack.o $(CFLAGS)
$(LIB)/vector.o: $(SRC)/vector.c $(INCLUDE)/vector.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/vector.c -o $(LIB)/vector.o $(CFLAGS)
$(LIB)/nd$(AR)ray.o: $(SRC)/nd$(AR)ray.c $(INCLUDE)/nd$(AR)ray.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/nd$(AR)ray.c -o $(LIB)/nd$(AR)ray.o $(CFLAGS)
$(LIB)/func.o: $(SRC)/func.c $(INCLUDE)/func.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/func.c -o $(LIB)/func.o $(CFLAGS)
$(LIB)/collision.o: $(SRC)/collision.c $(INCLUDE)/collision.h
	@mkdir -p $(LIB)
	$(CC) -c $(SRC)/collision.c -o $(LIB)/collision.o $(CFLAGS)

clean:
	rm -rf lib sample
run: sample
	./sample
