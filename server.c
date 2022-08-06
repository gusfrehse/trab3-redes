#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/ethernet.h>

#include "ConexaoRawSocket.h"

int main() {
  printf("server!\n");

  int s = ConexaoRawSocket("lo");

  char buf[2048];

  for (;;) {
    printf("esperando...\n");
    int result = recv(s, buf, sizeof(buf), 0);
    if (result < 0) {
      perror("recv");
      exit(1);
    }

    buf[result] = '\0';
    printf("recebi %d bytes '%s'\n", result, buf);
  }

  return 0;
}
