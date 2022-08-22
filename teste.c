#include <stdio.h>
#include "jogo.h"

int main() {
  jogador j;

  j.dados[0] = 1;
  j.dados[1] = 1;
  j.dados[2] = 1;
  j.dados[3] = 1;
  j.dados[4] = 5;

  printar_dados(&j);
  if (verifica_jogada(JOG_QUADRA, j.dados)) {
    printf("\taceito\n");
  } else {
    printf("\tnao aceito\n");
  }

  j.dados[0] = 1;
  j.dados[1] = 6;
  j.dados[2] = 6;
  j.dados[3] = 6;
  j.dados[4] = 6;

  printar_dados(&j);
  if (verifica_jogada(JOG_QUADRA, j.dados)) {
    printf("\taceito\n");
  } else {
    printf("\tnao aceito\n");
  }

  j.dados[0] = 5;
  j.dados[1] = 5;
  j.dados[2] = 5;
  j.dados[3] = 5;
  j.dados[4] = 5;

  printar_dados(&j);
  if (verifica_jogada(JOG_QUADRA, j.dados)) {
    printf("\taceito\n");
  } else {
    printf("\tnao aceito\n");
  }

  j.dados[0] = 1;
  j.dados[1] = 2;
  j.dados[2] = 3;
  j.dados[3] = 4;
  j.dados[4] = 5;

  printar_dados(&j);
  if (verifica_jogada(JOG_QUADRA, j.dados)) {
    printf("\taceito\n");
  } else {
    printf("\tnao aceito\n");
  }
}
