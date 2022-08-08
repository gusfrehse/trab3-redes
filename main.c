#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rede.h"

#include <sys/socket.h>

int main() {
  printf("server!\n");

  int porta_saida, porta_entrada, bastao;

  printf("porta saida: ");
  scanf("%d", &porta_saida);

  printf("porta entrada: ");
  scanf("%d", &porta_entrada);

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

  int ss = socket_sender(porta_saida, "127.0.0.1");
  int sr = socket_receiver(porta_entrada);

  char buf[2048];

  strcpy(buf, "teste teste 123 123");

  if (bastao) {
    for (;;) {
      printf("ENTER pra mandar msg\n");
      getchar();

      int mandado = send(ss, buf, strlen(buf), 0);
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
