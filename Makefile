

CFLAGS = -g -std=c11

all : client server

client : client.c rede.o rede.h
server : server.c rede.o rede.h
rede.o : rede.h

.PHONY : clean
clean :
	rm -rf *.o
	rm -rf client
	rm -rf server

