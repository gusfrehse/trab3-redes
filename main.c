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

  int ss = socket_sender(minha_porta_saida, "127.0.0.1");
  int sr = socket_receiver(minha_porta_entrada);

  char buf[2048];

  strcpy(buf, "teste teste 123 123");

  if (bastao) {
    for (;;) {
      printf("ENTER pra mandar msg\n");
      getchar();

      int mandado = send(ss, buf, strlen(buf), 0);
      if (mandado < 0) {
        perror("send");
      }
      printf("mandei %d bytes!\n", mandado);
    }
  } else {
    for (;;) {
      printf("esperando msg...\n");

      int recebido = recv(sr, buf, sizeof(buf), 0);
      buf[sizeof(buf) - 1] = '\0';
      printf("recebido %d bytes: '%s'!\n", recebido, buf);
    }
  }

  return 0;
}
