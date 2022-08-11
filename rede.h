#ifndef REDE_H
#define REDE_H

#include <arpa/inet.h>

#define TIPO_ATUALIZACAO 1
#define TIPO_APOSTA 2
#define TIPO_JOGAR 3
#define TIPO_ATUALIZACAO_FICHAS 4
#define TIPO_BASTAO 5

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

void inicializa_soquete(int porta_envio, int porta_recebe, char *ip_envio);
void enviar_mensagem(char tipo_msg, char jogador, char aposta, char jogada);
mensagem receber_mensagem();

#endif
