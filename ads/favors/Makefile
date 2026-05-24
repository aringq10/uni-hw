programa.exe : main.c graph.h graph.o Deque.o
	gcc -std=c99 -Wall -o programa.exe main.c graph.o Deque.o

graph.o : graph.c graph.h Deque.h
	gcc -c -std=c99 -Wall graph.c

Deque.o : Deque.c Deque.h
	gcc -c -std=c99 -Wall Deque.c
