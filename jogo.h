#ifndef JOGO_H
#define JOGO_H

#define NUM_DADOS 5
#define JOG_PAR 1
#define JOG_TRIO 2
#define JOG_DOIS_PAR 3
#define JOG_FULL_HOUSE 4
#define JOG_SEQ_BAIXA 5
#define JOG_SEQ_ALTA 6
#define JOG_QUADRA 7
#define JOG_QUINTETO 8

typedef struct jogador {
    int num_fichas;
    int jogada;
    long porta_entrada;
    long porta_saida;
    short num_jogador;
    short dados[NUM_DADOS];
    short dado_bloqueado[NUM_DADOS];
} jogador;

void jogar_dados(jogador* jogador);
void printar_dados(jogador* jogador);
void ordenar_dados(short *dados);
void bloquear_dados(jogador* jogador);
void mostrar_jogadas();
int verifica_jogada(int jogada, short *dados);

#endif
