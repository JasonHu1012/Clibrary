CC=gcc

liblist.a: list.o lnode.o
	ar rcs liblist.a list.o lnode.o
libmatrix.a: matrix.o
	ar rcs libmatrix.a matrix.o
libqueue.a: queue.o lnode.o
	ar rcs libqueue.a queue.o lnode.o
libstack.a: stack.o lnode.o
	ar rcs libstack.a stack.o lnode.o
libvector.a: vector.o
	ar rcs libvector.a vector.o
libndarray.a: ndarray.o
	ar rcs libndarray.a ndarray.o
libfunc.a: func.o
	ar rcs libfunc.a func.o

list.o: list.c list.h
	$(CC) -c list.c -o list.o
lnode.o: lnode.c lnode.h
	$(CC) -c lnode.c -o lnode.o
matrix.o: matrix.c matrix.h
	$(CC) -c matrix.c -o matrix.o
queue.o: queue.c queue.h
	$(CC) -c queue.c -o queue.o
stack.o: stack.c stack.h
	$(CC) -c stack.c -o stack.o
vector.o: vector.c vector.h
	$(CC) -c vector.c -o vector.o
ndarray.o: ndarray.c ndarray.h
	$(CC) -c ndarray.c -o ndarray.o
func.o: func.c func.h
	$(CC) -c func.c -o func.o

clean:
	rm -f *.a *.o sample
run: sample
	./sample
sample: sample.c liblist.a libmatrix.a libqueue.a libstack.a libvector.a libndarray.a libfunc.a
	$(CC) sample.c -L . -lm -llist -lmatrix -lqueue -lstack -lvector -lndarray -lfunc -o sample
