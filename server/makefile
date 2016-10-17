CFLAGS= -g -Wall
server: server.o
server.o: server.c
fifo:
	mkfifo /tmp/disp
	mkfifo /tmp/rfifo
clean:
	rm *.o server

