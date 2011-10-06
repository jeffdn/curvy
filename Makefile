# curvy

CC = gcc
CFLAGS = -g -pipe -O2 -W -Wall -pedantic

all: curvy

curvy.o: src/curvy.h src/curvy.c
	${CC} ${CFLAGS} -c src/curvy.c

main.o: src/main.c
	${CC} ${CFLAGS} -c src/main.c

curvy: curvy.o main.o
	${CC} ${CFLAGS} -lm -lgd curvy.o main.o -o curvy

clean:
	rm -f *.o curvy

