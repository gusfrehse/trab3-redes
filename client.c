#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <string.h>

#include "ConexaoRawSocket.h"

int main() {
  printf("client!\n");

  int s = ConexaoRawSocket("lo");

  char *buf = "teste123";

  int mandado = send(s, buf, strlen(buf), 0);
  if (mandado < 0) {
    perror("send");
  }

  return 0;
}
