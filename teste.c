#include <stdio.h>
#include "jogo.h"

int main() {
  jogador j;
  j.dados[0] = 1;
  j.dados[1] = 2;
  j.dados[2] = 2;
  j.dados[3] = 5;
  j.dados[4] = 5;

  printar_dados(&j);
  if (verifica_jogada(JOG_DOIS_PAR, j.dados)) {
    printf("deu bom\n");
  } else {
    printf("deu ruim\n");
  }

  printar_dados(&j);
}
