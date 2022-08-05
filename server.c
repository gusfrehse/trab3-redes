#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/ethernet.h>

int main() {
  printf("server!\n");

  int s = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL));
  if (s < 0) {
    perror("socket");
    exit(1);
  }


  char buf[2048];

  for (;;) {
    int result = recv(s, buf, sizeof(buf), 0);
    if (result < 0) {
      perror("recv");
      exit(1);
    }

    buf[sizeof(buf) -1] = '\0';
    printf("recebi '%s'\n", buf);
  }

  return 0;
}
