#include "rede.h"

#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BYTE_INIT (0b01010101)

int sequencia_envio = 0;
int sequencia_recebe = 0;

int soquete_envio;
int soquete_recebe;

static int socket_receiver(in_port_t porta);
static int socket_sender(in_port_t porta, char *ip_str);

void inicializa_soquete(int porta_envio, int porta_recebe, char *ip_envio) {
  soquete_recebe = socket_receiver(porta_recebe);
  soquete_envio = socket_sender(porta_envio, ip_envio);
}

static int socket_receiver(in_port_t porta) {
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

void enviar_mensagem(char tipo_msg, char jogador, char aposta, char jogada){
  mensagem msg;
  msg.inicializacao = BYTE_INIT;
  msg.tamanho = sizeof(mensagem);
  msg.sequencia = sequencia_envio++;
  msg.tipo_msg = tipo_msg;
  msg.jogador = jogador;
  msg.valor_aposta = aposta;
  msg.tipo_jogada = jogada;
  msg.paridade = msg.inicializacao ^ msg.tamanho ^ msg.sequencia ^ msg.tipo_msg ^ msg.jogador ^ (unsigned char) msg.valor_aposta ^ msg.tipo_jogada;

  int enviados = send(soquete_envio, &msg, sizeof(msg), 0);
  if (enviados < 0) {
    perror("send");
    exit(1);
  }
}

static void enviar_erro() {
  mensagem msg = {};
  msg.inicializacao = BYTE_INIT;
  msg.tipo_msg = TIPO_ERRO;

  send(soquete_envio, &msg, sizeof(msg), 0);
}

mensagem receber_mensagem() {
  // TODO: checar paridade
  mensagem msg;

  int recebidos = recv(soquete_recebe, &msg, sizeof(msg), 0);
  //printf("recebi %d bytes\n", recebidos);
  if (recebidos < 0) {
    perror("recv");
    exit(1);
  }

  if (msg.tipo_msg == TIPO_ERRO) {
    printf("ERRO em máquina anterior. Abortando.\n");
    enviar_erro();
    exit(11);
  }

  unsigned char paridade = msg.inicializacao ^ msg.tamanho ^ msg.sequencia ^ msg.tipo_msg ^ msg.jogador ^ (unsigned char) msg.valor_aposta ^ msg.tipo_jogada;
  if (paridade != msg.paridade) {
    printf("ERRO de paridade. Abortando.\n");
    printf("Esperava 0x%x obtive 0x%x\n", paridade, msg.paridade);
    enviar_erro();
    exit(22);
  }

  // sequencializacao
  if (sequencia_recebe++ != msg.sequencia) {
    printf("ERRO de sequencialização. Abortando.\n");
    printf("Esperava %u obtive %u\n", sequencia_recebe, msg.sequencia);
    enviar_erro();
    exit(33);
  }

  return msg;
}

