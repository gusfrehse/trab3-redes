#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rede.h"
#include "jogo.h"

#include <sys/socket.h>

#define NUM_MAQ 4
#define FICHAS_INICIAIS 15

int main(int argc, char *argv[]) {
  jogador eu = {}; 

  eu.num_fichas = FICHAS_INICIAIS;

  eu.num_jogador = atoi(argv[argc - 1]);
  printf("MAQUINA: %d\n", eu.num_jogador);

  int porta_saida[NUM_MAQ], porta_entrada[NUM_MAQ], bastao;

  for(int i = 1;i <= NUM_MAQ;i++)
    porta_saida[i - 1] = atoi(argv[i]);
    
  porta_entrada[0] = porta_saida[NUM_MAQ - 1];
  for(int i = 1;i < NUM_MAQ;i++)
    porta_entrada[i] = porta_saida[i - 1];

  eu.porta_saida = porta_saida[eu.num_jogador - 1];
  eu.porta_entrada = porta_entrada[eu.num_jogador - 1];

  printf("PORTA DE ENTRADA: %ld\n", eu.porta_entrada);
  printf("PORTA DE SAIDA: %ld\n", eu.porta_saida);

  inicializa_soquete(eu.porta_saida, eu.porta_entrada, "127.0.0.1");

  // checa bastao
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

  if (bastao) {
    printf("Comecei com o bastão!\n");
    printf("enviando msg\n");
    mostrar_jogadas();
    scanf("%d", &eu.jogada);
    enviar_mensagem(TIPO_APOSTA, 1, 1, eu.jogada);

    mensagem msg = receber_mensagem();
    enviar_mensagem(TIPO_ATUALIZACAO, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
    printf("recebi a mensagem de volta\n");

    msg = receber_mensagem();

  } else {
    printf("Não comecei com o bastão!\n");

    printf("esperando msg\n");
    mensagem msg = receber_mensagem();
    printf("recebi uma msg!\n");
    printf("jogada eh %d, do jogador %d, com aposta de %d\n", msg.tipo_jogada, msg.jogador, msg.valor_aposta);
    eu.jogada = msg.tipo_jogada;
    printf("deseja apostar?[s/n] ");
    char char_aposta;
    scanf(" %c", &char_aposta);
    //printf("char_aposta = '%c'", char_aposta);
    //printf("jogador: %d\n", eu.num_jogador);
    if (char_aposta == 's' || char_aposta == 'S') {
      printf("apostado!\n");

      enviar_mensagem(msg.tipo_msg, eu.num_jogador, msg.valor_aposta + 1, msg.tipo_jogada);
    } else {
      enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
    }

    msg = receber_mensagem();
    enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
    if (msg.jogador == eu.num_jogador) {
      for (int i = 0; i < 3; i++) {
        printf("Jogada [%d/3]\n", i + 1);
        jogar_dados(&eu);
        printar_dados(&eu);
        if(i < 2)
          bloquear_dados(&eu);
      }
      //printar_dados(&eu);
      ordenar_dados(eu.dados);
      //printf("Dados ordenados: \n");
      //printar_dados(&eu);
      int conseguiu = verifica_jogada(eu.jogada, eu.dados);
      if(conseguiu)
        printf("Voce venceu!!!\n");
      else
        printf("Nao foi dessa vez...\n");
    }
    else {
      enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
    }
  }

  return 0;
}
