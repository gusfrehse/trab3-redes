#ifndef JOGO_H
#define JOGO_H

#define NUM_DADOS 5
#define JOG_PAR 10
#define JOG_TRIO 11
#define JOG_DOIS_PAR 12
#define JOG_FULL_HOUSE 13
#define JOG_SEQ_BAIXA 14
#define JOG_SEQ_ALTA 15
#define JOG_QUADRA 16
#define JOG_QUINTETO 17

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
void printar_dados(short *dados);
void ordenar_dados(short *dados);
void bloquar_dados(short *dados_bloq);
int escolher_jogada(int num);


#endif