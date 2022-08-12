#include "jogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int jogar_dado() {
    return ((rand() % 6) + 1);
}

static int verifica_jogada_tripla(short *dados) {
    int trio = 0;
    for(int i = 0;i < NUM_DADOS - 2;i++){
        if((dados[i] == dados[i + 1]) && (dados[i + 1] == dados[i + 2]))
            trio = 1;
    }
    return trio;
}

void jogar_dados(jogador* jogador) {
    srand(time(NULL)); 
    printf("Digite qualquer letra para jogar os dados!\n");
    if(getchar() == '\n')
        getchar();
    for(int i = 0;i < NUM_DADOS;i++)
        if(jogador->dado_bloqueado[i] == 0)
            jogador->dados[i] = jogar_dado();
}
void printar_dados(jogador *jogador) {
    for(int i = 0;i < NUM_DADOS;i++){
        if(jogador->dado_bloqueado[i] == 1)
            printf("[%d]* ", jogador->dados[i]);
        else
            printf("[%d] ", jogador->dados[i]);
    }
    printf("\n");   
}

static void troca(short *v, short num1, short num2){
    short aux;
    aux = v[num1];
    v[num1] = v[num2];
    v[num2] = aux;
}

static short min_vetor(short *v, int num) {
    short min = num;
    for(int i = num;i < NUM_DADOS;i++)
        if(v[i] < v[min])
            min = i;
    return min;
}

void ordenar_dados(short *dados) {
    int i, j, min;
    for(i = 0;i < NUM_DADOS;i++){
        min = min_vetor(dados, i);
        troca(dados, min, i);
    }
}

void bloquear_dados(jogador *jogador) {
    char bloq;
    for(int i = 0;i < NUM_DADOS;i++){
        if(jogador->dado_bloqueado[i] == 0){
            printf("Bloquar dado %d? -> [%d] - [s/n]: ", i + 1, jogador->dados[i]);
            scanf(" %c", &bloq);
            if(bloq == 's' || bloq == 'S')
                jogador->dado_bloqueado[i] = 1;
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
    printf("Digite o numero da jogada que deseja fazer: ");
}

static int verifica_jogada_par(short *dados) {
  for (int i = 1; i < NUM_DADOS; i++) {
    if (dados[i-1] == dados[i])
      return 1;
  }

  return 0;
}

static int verifica_jogada_dois_par(short *dados) {
  int primeiro_par = 0;
  for (int i = 0; i < NUM_DADOS - 1; i++) {
    if (dados[i-1] == dados[i])
      primeiro_par = i;
  }

  if (!primeiro_par)
    return 0;

  for (int i = NUM_DADOS - 2; dados[primeiro_par] != dados[i]; i--) {
    if (dados[i] == dados[i + 1]) 
      return 1;
  }

  return 0;
}

static int verifica_jogada_fullhouse(short *dados) {
    int fh = 0;
    if(verifica_jogada_dois_par(dados) && verifica_jogada_tripla(dados))
        fh = 1;
    return fh;
}

int verifica_jogada(int jogada, short *dados) {
  int conseguiu = 0;  
  switch (jogada) {
  case JOG_PAR: conseguiu = verifica_jogada_par(dados); break;
  case JOG_TRIO: conseguiu = verifica_jogada_tripla(dados); break;
  case JOG_DOIS_PAR: conseguiu = verifica_jogada_dois_par(dados); break;
  case JOG_FULL_HOUSE: conseguiu = verifica_jogada_fullhouse(dados); break;
  case JOG_SEQ_BAIXA: break;
  case JOG_SEQ_ALTA: break;
  case JOG_QUADRA: break;
  case JOG_QUINTETO: break;
  }
  return conseguiu;
}
