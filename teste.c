#include <stdio.h>
#include "jogo.h"

int main() {
  jogador j;
  j.dados[0] = 1;
  j.dados[1] = 1;
  j.dados[2] = 5;
  j.dados[3] = 5;
  j.dados[4] = 5;

  printar_dados(&j);
  if (verifica_jogada(JOG_FULL_HOUSE, j.dados)) {
    printf("deu bom\n");
  } else {
    printf("deu ruim\n");
  }

  printar_dados(&j);
}
