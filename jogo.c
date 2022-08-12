#include "jogo.h"
#include <stdio.h>
#include <stdlib.h>

static int jogar_dado() {
    return ((rand() % 6) + 1);
}

void jogar_dados(jogador* jogador) {
    for(int i = 0;i < NUM_DADOS;i++)
        if(jogador->dado_bloqueado[i] == 0)
            jogador->dados[i] = jogar_dado();
}
void printar_dados(short *dados) {
    for(int i = 0;i < NUM_DADOS;i++)
        printf("%d ", dados[i]);
}

static void troca(short *v, short num1, short num2){
    short aux;
    aux = v[num1];
    v[num1] = v[num2];
    v[num2] = aux;
}

static short min_vetor(short *v) {
    short min = 0;
    for(int i = 1;i < NUM_DADOS;i++)
        if(v[i] < v[min])
            min = i;
    return min;
}

void ordenar_dados(short *dados) {
    int i, j, min;
    for(i = 0;i < NUM_DADOS;i++){
        min = min_vetor(dados);
        troca(dados, min, i);
    }
}

void bloquar_dados(short *dados_bloq) {
    char bloq;
    for(int i = 0;i < NUM_DADOS;i++){
        if(dados_bloq[i] == 0){
            printf("Bloquar dado %d?[s/n] \n", i + 1);
            scanf("%c", &bloq);
            if(bloq == 's' || bloq == 'S')
                dados_bloq[i] = 1;
        }
    }       
}

void mostrar_jogadas() {
    printf("---------- JOGADAS ----------\n");
    printf("1 - Dupla --> Ganho em fichas: 2\n");
    printf("2 - Trio --> Ganho em fichas: 3\n");
    printf("3 - Dois pares --> Ganho em fichas: 4\n");
    printf("4 - Full House (1 Par 1 Trio) --> Ganho em fichas:  5\n");
    printf("5 - Sequencia Baixa (1..5) --> Ganho em fichas: 7\n");
    printf("6 - Sequencia Alta (2..6) --> Ganho em fichas: 7\n");
    printf("7 - Quadra --> Ganho em fichas: 10\n");
    printf("8 - Quinteto --> Ganho em fichas: 15\n");
    printf("Digite o numero da jogada que deseja fazer e o numero de fichas que ira apostar: ");
}