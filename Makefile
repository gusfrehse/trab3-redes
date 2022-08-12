

CFLAGS = -g -std=c11

main : rede.o jogo.o

rede.o : rede.h

jogo.o : jogo.h

.PHONY : clean
clean :
	rm -f *.o
	rm -f main

