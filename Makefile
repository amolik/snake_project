project: main.o snake.o
	cc main.o snake.o -lm -o project
main.o: main.c snake.h
	cc -Wall -c main.c
snake.o: snake.c snake.h 
	cc -Wall -c snake.c 



