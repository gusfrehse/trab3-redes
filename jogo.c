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
  int primeiro_par = -1;
  for (int i = 1; i < NUM_DADOS; i++) {
    if (dados[i - 1] == dados[i]) {
      primeiro_par = i;
      break;
    }
  }

  if (primeiro_par == -1)
    return 0;

  for (int i = primeiro_par + 1; i < NUM_DADOS - 1; i++) {
    if (dados[i] == dados[i + 1]) 
      return 1;
  }

  return 0;
}

static int verifica_jogada_fullhouse(short *dados) {
  int nums[6] = {};
  for (int i = 0; i < NUM_DADOS; i++) {
    nums[dados[i] - 1]++;
  }

  int trinca = 0;
  int par = 0;

  for (int i = 0; i < 6; i++) {
    printf("DEBUG verifica fullhouse: dados com %d = %d\n", i, nums[i]);
    if (nums[i] == 3) {
      trinca = 1;
    } else if (nums[i] == 2) {
      par = 1;
    }
  }

  return trinca && par;
}

static int verifica_jogada_seq_baixa(short *dados) {
  for (int i = 0; i < NUM_DADOS; i++) {
    if (dados[i] != i + 1)
      return 0;
  }

  return 1;
}

static int verifica_jogada_seq_alta(short *dados) {
  for (int i = 0; i < NUM_DADOS; i++) {
    if (dados[i] != i + 2)
      return 0;
  }

  return 1;
}

static int verifica_jogada_quadra(short *dados) {
  int nums[6] = {};
  for (int i = 0; i < NUM_DADOS; i++) {
    nums[dados[i] - 1]++;
  }

  for (int i = 0; i < 6; i++) {
    if (nums[i] == 4) {
      return 1;
    }
  }

  return 0;
}

static int verifica_jogada_quinteto(short *dados) {
  int nums[6] = {};
  for (int i = 0; i < NUM_DADOS; i++) {
    nums[dados[i] - 1]++;
  }

  for (int i = 0; i < 6; i++) {
    if (nums[i] == 5) {
      return 1;
    }
  }

  return 0;
}

void executar_jogada(jogador* jogador){
  for (int i = 0; i < 3; i++) {
    printf("Jogada [%d/3]\n", i + 1);
    jogar_dados(jogador);
    printar_dados(jogador);
    if(i < 2)
      bloquear_dados(jogador);
  }
}

char* num2jogada(int num){
  switch (num) {
  case JOG_PAR: return "Dupla"; break;
  case JOG_TRIO: return "Tripla"; break;
  case JOG_DOIS_PAR: return "Dois pares"; break;
  case JOG_FULL_HOUSE: return "Full House"; break;
  case JOG_SEQ_BAIXA: return "Sequencia baixa (1..5)"; break;
  case JOG_SEQ_ALTA: return "Sequencia alta (2..6)"; break;
  case JOG_QUADRA: return "Quadra"; break;
  case JOG_QUINTETO: ; return "Quinteto"; break;
  }
}

int valor_premio_jogada(int num) {
  switch (num) {
  case JOG_PAR: return 2; break;
  case JOG_TRIO: return 3; break;
  case JOG_DOIS_PAR: return 4; break;
  case JOG_FULL_HOUSE: return 5; break;
  case JOG_SEQ_BAIXA: return  7; break;
  case JOG_SEQ_ALTA: return 7; break;
  case JOG_QUADRA: return 10; break;
  case JOG_QUINTETO: ; return 15; break;
  }
}

int verifica_jogada(int jogada, short *dados) {
  ordenar_dados(dados);

  int conseguiu = 0;  
  switch (jogada) {
  case JOG_PAR: conseguiu = verifica_jogada_par(dados); break;
  case JOG_TRIO: conseguiu = verifica_jogada_tripla(dados); break;
  case JOG_DOIS_PAR: conseguiu = verifica_jogada_dois_par(dados); break;
  case JOG_FULL_HOUSE: conseguiu = verifica_jogada_fullhouse(dados); break;
  case JOG_SEQ_BAIXA: conseguiu = verifica_jogada_seq_baixa(dados); break;
  case JOG_SEQ_ALTA: conseguiu = verifica_jogada_seq_alta(dados); break;
  case JOG_QUADRA: conseguiu = verifica_jogada_quadra(dados); break;
  case JOG_QUINTETO: conseguiu = verifica_jogada_quinteto(dados); break;
  }

  return conseguiu;
}

int ganho_jogada (int jogada) {
  switch (jogada) {
  case JOG_PAR: return 2;
  case JOG_TRIO: return 3;
  case JOG_DOIS_PAR: return 4;
  case JOG_FULL_HOUSE: return 5;
  case JOG_SEQ_BAIXA: return 7;
  case JOG_SEQ_ALTA: return 7;
  case JOG_QUADRA: return 10;
  case JOG_QUINTETO: return 15;
  default: return 0;
  }
}
