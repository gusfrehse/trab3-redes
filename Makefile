

CFLAGS = -g -std=c11

all : client server

client : client.c ConexaoRawSocket.o
server : server.c ConexaoRawSocket.o

ConexaoRawSocket.o : ConexaoRawSocket.h

.PHONY : clean
clean :
	rm -rf *.o
	rm -rf client
	rm -rf server

