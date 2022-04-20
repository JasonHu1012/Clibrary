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

clean:
	rm -f *.a *.o sample
run: sample
	./sample
sample: sample.c liblist.a libmatrix.a libqueue.a libstack.a libvector.a
	gcc sample.c -L . -llist -lmatrix -lqueue -lstack -lvector -o sample
