mytest: add.o sub.o
	gcc -o mytest add.o sub.o

add.o: add.c test.h
	gcc -c add.c

sub.o: sub.c test.h
	gcc -c sub.c

clean:
	rm -rf mytest
	rm -rf *.o
