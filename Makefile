

CFLAGS = -g -std=c11

main : rede.o

rede.o : rede.h

.PHONY : clean
clean :
	rm -f *.o
	rm -f main

