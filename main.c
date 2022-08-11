#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rede.h"

#include <sys/socket.h>

#define NUM_MAQ 4

int main(int argc, char *argv[]) {

  int maq = atoi(argv[argc - 1]);
  printf("MAQUINA: %d\n", maq);

  int porta_saida[NUM_MAQ], porta_entrada[NUM_MAQ], bastao;

  for(int i = 1;i <= NUM_MAQ;i++)
    porta_saida[i - 1] = atoi(argv[i]);
  
    
  porta_entrada[0] = porta_saida[NUM_MAQ - 1];
  //porta_saida[argc - 1] = porta_entrada[0];
  for(int i = 1;i < NUM_MAQ;i++)
    porta_entrada[i] = porta_saida[i - 1];

  for(int i = 1;i <= NUM_MAQ;i++)
    printf("Porta saida: %d - Porta de entrada: %d\n", porta_saida[i - 1], porta_entrada[i - 1]);

  int minha_porta_saida = porta_saida[maq - 1];
  int minha_porta_entrada = porta_entrada[maq - 1];

  printf("PORTA DE ENTRADA: %d\n", minha_porta_entrada);
  printf("PORTA DE SAIDA: %d\n", minha_porta_saida);

  inicializa_soquete(minha_porta_entrada, minha_porta_saida, "127.0.0.1");

  char char_bastao;
  printf("bastao? [y/N] ");
  char_bastao = getchar();
  if (char_bastao == '\n')
    char_bastao = getchar();

  if (char_bastao == 'y') {
    bastao = 1;
  } else {
    bastao = 0;
  }

  printf("bastao é %d\n", bastao);

  if (bastao) {
    // TODO: coletar qual o tipo da jogada atual
    int jogada = 1;

    printf("enviando msg\n");
    enviar_mensagem(TIPO_APOSTA, maq, 1, jogada);
    mensagem msg = receber_mensagem();
    printf("recebi a mensagem de volta\n");
  } else {
    printf("esperando msg\n");
    mensagem msg = receber_mensagem();
    printf("recebi uma msg!\n");
    if (msg.jogador == maq) {
      // faz algo
      printf("pra mim\n");
    } else {
      printf("nao eh pra mim\n");

      enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
    }
  }

  return 0;
}
