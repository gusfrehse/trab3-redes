#include "rede.h"

#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int socket_receiver(in_port_t porta) {
  int soc = socket(AF_INET, SOCK_DGRAM, 0); /*cria socket*/
  if (soc < 0) {
    perror("socket");
    exit(-1);
  }

  struct sockaddr_in endereco;
  endereco.sin_family = AF_INET;
  endereco.sin_port = htons(porta);
  endereco.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(soc, (struct sockaddr *)&endereco, sizeof(endereco)) == -1) {
    perror("bind");
    exit(1);
  }

  return soc;
}

int socket_sender(in_port_t porta, char *ip_str) {
  int soc = socket(AF_INET, SOCK_DGRAM, 0); /*cria socket*/
  if (soc < 0) {
    perror("socket");
    exit(-1);
  }

  struct sockaddr_in endereco;
  endereco.sin_family = AF_INET;
  endereco.sin_port = htons(porta);
  endereco.sin_addr.s_addr = inet_addr(ip_str);

  int result = connect(soc, (struct sockaddr *)&endereco, sizeof(endereco));
  if (result < 0) {
    perror("connect");
    exit(1);
  }

  return soc;
}

void enviar_mensagem(int socket, char tipo_msg, char jogador, char aposta, char jogada){
  mensagem msg;
  msg.tipo_msg = tipo_msg;
}