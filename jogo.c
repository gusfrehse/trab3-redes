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
void ordenar_dados(short *dados) {

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