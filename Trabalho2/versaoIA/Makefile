CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c23

OBJS = envio.o main.o

all: envio

envio: $(OBJS)
	$(CC) $(CFLAGS) -o envio $(OBJS)

envio.o: envio.c envio.h
	$(CC) $(CFLAGS) -c envio.c

main.o: main.c envio.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f envio *.o
