#ifndef REDE_H
#define REDE_H

#include <arpa/inet.h>

typedef struct mensagem {
    unsigned char inicializacao;
    unsigned char tamanho;
    unsigned char sequencia;
    unsigned char tipo_msg;
    unsigned char jogador;
    unsigned char valor_aposta;
    unsigned char tipo_jogada; 
    unsigned char paridade;
} mensagem; 

int socket_receiver(in_port_t porta);
int socket_sender(in_port_t porta, char *ip_str);
void enviar_mensagem(int socket, char tipo_msg, char jogador, char aposta, char jogada);

#endif
