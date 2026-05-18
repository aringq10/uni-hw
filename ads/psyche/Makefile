main.exe: main.o psyche.o Deque.o BigInt.o
	gcc -std=c99 main.o psyche.o Deque.o BigInt.o -o main.exe

main.o: main.c psyche.h Deque.h BigInt.h
	gcc -std=c99 -c main.c

psyche.o: psyche.c psyche.h Deque.h BigInt.h
	gcc -std=c99 -c psyche.c

Deque.o: Deque.c Deque.h
	gcc -std=c99 -c Deque.c

BigInt.o: BigInt.c BigInt.h
	gcc -std=c99 -c BigInt.c

clean:
	rm -f *.o main.exe main result*.txt test*.cfg
