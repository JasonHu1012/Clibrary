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

list.o: list.c list.h
	gcc -c list.c -o list.o
lnode.o: lnode.c lnode.h
	gcc -c lnode.c -o lnode.o
matrix.o: matrix.c matrix.h
	gcc -c matrix.c -o matrix.o
queue.o: queue.c queue.h
	gcc -c queue.c -o queue.o
stack.o: stack.c stack.h
	gcc -c stack.c -o stack.o
vector.o: vector.c vector.h
	gcc -c vector.c -o vector.o

clean:
	rm -f *.a *.o sample
run: sample
	./sample
sample: sample.c liblist.a libmatrix.a libqueue.a libstack.a libvector.a
	gcc sample.c -L . -llist -lmatrix -lqueue -lstack -lvector -o sample
