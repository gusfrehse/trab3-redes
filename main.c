#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "rede.h"
#include "jogo.h"

#include <sys/socket.h>

#define NUM_MAQ 4
#define FICHAS_INICIAIS 15
#define APOSTA_INICIAL 1

jogador eu = {}; 
int bastao = 0;

int jogar_jogada() {
      executar_jogada(&eu);
      //printar_dados(&eu);
      ordenar_dados(eu.dados);
      printf("Dados ordenados: \n");
      printar_dados(&eu);
      int conseguiu = verifica_jogada(eu.jogada, eu.dados);
      if(conseguiu) {
        printf("Voce venceu!!!\n");
        return 1;
      }

      printf("Nao foi dessa vez...\n");
      return 0;
}

void fluxo_bastao() {
  mensagem msg;
  int apostador;
  int valor_aposta;
  int jogada;

  // usuario escolhe a jogada
  mostrar_jogadas();
  scanf("%d", &jogada);

  // inicia as apostas
  enviar_mensagem(TIPO_APOSTA, eu.num_jogador, APOSTA_INICIAL, jogada);

  // obtem o maior apostador
  msg = receber_mensagem();
  assert(msg.tipo_msg == TIPO_APOSTA);

  apostador = msg.jogador;
  valor_aposta = msg.valor_aposta;

  // atualiza os jogadores de quem mais apostou
  enviar_mensagem(TIPO_ATUALIZACAO, apostador, valor_aposta, jogada);

  // recebe de volta a mensagem
  msg = receber_mensagem();
  assert(msg.tipo_msg == TIPO_ATUALIZACAO);

  // passa o pseudo bastao para o apostador
  enviar_mensagem(TIPO_JOGAR, apostador, valor_aposta, jogada);
  printf("mandei TIPO_JOGAR apostador %d valor_aposta %d jogada %s\n", apostador, valor_aposta, num2jogada(jogada));

  // recebe atualizacao das fichas do apostador
  msg = receber_mensagem();
  assert(msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);

  // repassa TIPO_ATUALIZACAO_FICHAS
  enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

  // passa o bastao para o proximo jogador (TODO: nao sei o que passar aqui nos outros campos)
  printf("mandei bastao\n");
  enviar_mensagem(TIPO_BASTAO, 0, 0, 0);

  bastao = 0;
}

int perguntar_aposta(int apostador, int tipo_jogada, int aposta_atual, int nova_aposta) {
  printf("(Jogador %d) A jogada da vez eh: %s - Aposta atual: %d\n", apostador, num2jogada(tipo_jogada), aposta_atual);
  printf("deseja apostar %d fichas? [s/n] ", aposta_atual + 1);
  
  char char_aposta;
  scanf(" %c", &char_aposta);

  return char_aposta == 's' || char_aposta == 'S';
}

void fluxo_nao_bastao() {
  // recebe a mensagem de aposta
  mensagem msg = receber_mensagem();
  assert(msg.tipo_msg == TIPO_APOSTA);

  // usuario escolhe se quer apostar
  eu.jogada = msg.tipo_jogada;

  int minha_aposta = msg.valor_aposta + 1;
  if (perguntar_aposta(msg.jogador, msg.tipo_jogada, msg.valor_aposta, minha_aposta)) {
    printf("apostado %d fichas!\n", minha_aposta);
    // encaminha minha aposta
    enviar_mensagem(msg.tipo_msg, eu.num_jogador, minha_aposta, msg.tipo_jogada);
  } else {
    minha_aposta = 0;
    // encaminha aposta
    enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
  }

  msg = receber_mensagem();
  assert(msg.tipo_msg == TIPO_ATUALIZACAO);
  printf("maior aposta foi do jogador %d com %d fichas na jogada %s\n", msg.jogador, msg.valor_aposta, num2jogada(msg.tipo_jogada));

  // reenvia a atualizacao
  enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

  // Essa pode ser tanto TIPO_JOGAR quanto TIPO_ATUALIZACAO_FICHAS,
  // se é ou está antes do jogador com maior aposta, vai ser TIPO_JOGAR
  // se está depois, vai ser TIPO_ATUALIZACAO_FICHAS
  msg = receber_mensagem(); 
  assert(msg.tipo_msg == TIPO_JOGAR || msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);
  printf("recebi JOGAR ou ATUALIZACAO FICHAS: ");
  printf("tipo %d jogador %d eu_num %d\n", msg.tipo_jogada, msg.jogador, eu.num_jogador);

  int sou_maior_apostador = 0;

  if (msg.tipo_msg == TIPO_JOGAR && msg.jogador == eu.num_jogador) {
      // sou o maior apostador, preciso:
      //  - jogar dados
      //  - mandar atualizacao de fichas
      //  - receber atualizacao de fichas
      //  - mandar bastao para o original

      sou_maior_apostador = 1;
      printf("sou o maior apostador!\n");

      char ganho = 0;

      if (jogar_jogada()) {
        ganho = 10; // TODO: mudar para o valor correto
      } else {
        ganho = -minha_aposta;
      }

      // manda atualizacao e espera dar a volta
      enviar_mensagem(TIPO_ATUALIZACAO_FICHAS, eu.num_jogador, ganho, msg.tipo_jogada);

      printf("esperando TIPO_ATUALIZACAO_FICHAS\n");
      msg = receber_mensagem();
      assert(msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);
      printf("recebi TIPO_ATUALIZACAO_FICHAS\n");

      // manda bastao, o original deve estar esperando.
      enviar_mensagem(TIPO_BASTAO, 0, 0, 0);

      // no proximo precisa esperar o bastao ou aposta

      return;
  }

  if (msg.tipo_msg == TIPO_JOGAR && msg.jogador != eu.num_jogador) {
    // nao sou, preciso repassar o TIPO_JOGAR e esperar a TIPO_ATUALIZACAO_FICHA
    printf("recebi jogar mas nao sou o maior apostador\n");
    enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

    msg = receber_mensagem();
    assert(msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);
  }

  // aqui msg é TIPO_ATUALIZACAO FICHAS
  //jogadores[msg.jogador - 1].fichas -= msg.valor_aposta;
  // TODO: atulizar as fichas do jogador da msg, checar se < 0 e sair se for o caso

  // repassa TIPO_ATUALIZACAO_FICHAS
  enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

  // no proximo precisa esperar bastao ou aposta
  return;
}

int main(int argc, char *argv[]) {

  eu.num_fichas = FICHAS_INICIAIS;

  eu.num_jogador = atoi(argv[argc - 1]);
  printf("MAQUINA: %d\n", eu.num_jogador);

  // Seleciona as portas
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

  // Cria o soquete
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
    fluxo_bastao();
    return 0;

    printf("Comecei com o bastão!\n");
    printf("enviando msg\n");
    mostrar_jogadas();
    scanf("%d", &eu.jogada);

    enviar_mensagem(TIPO_APOSTA, eu.num_jogador, APOSTA_INICIAL, eu.jogada);
    mensagem msg = receber_mensagem(); // recebe de volta a mensagem TIPO_APOSTA
                                       //
    printf("recebi a mensagem de volta\n");

    enviar_mensagem(TIPO_ATUALIZACAO, msg.jogador, msg.valor_aposta, msg.tipo_jogada); // comunica a rede do maior apostador

    msg = receber_mensagem(); // recebe de volta a mensagem TIPO_ATUALIZACAO
    if (msg.jogador != eu.num_jogador) {
      bastao = 0;

      // encaminha a mensagem para o proximo jogador
      enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

      // atualizacao do jogador de dados
      msg = receber_mensagem(); // TIPO_ATUALIZACAO_FICHAS
      printf("jogador %d ganhou/perdeu %d fichas\n", msg.jogador, msg.valor_aposta);
      // TODO: atualizar os dados do jogador msg.jogador
    } else {
      // é para mim a mensagem...
      bastao = 1;

      int ganho = 0;
      if (jogar_jogada()) {
        ganho = 10; // TODO: mudar para o valor correto
      } else {
        ganho = -APOSTA_INICIAL;
      }

      enviar_mensagem(TIPO_ATUALIZACAO_FICHAS, eu.num_jogador,  ganho, eu.jogada);
    }
  } else {
    printf("Não comecei com o bastão!\n");
    fluxo_nao_bastao();
    return 0;

    printf("esperando msg\n");
    mensagem msg = receber_mensagem(); // TIPO_APOSTA
    //printf("recebi uma msg!\n");
    //printf("jogada eh %d, do jogador %d, com aposta de %d\n", msg.tipo_jogada, msg.jogador, msg.valor_aposta);
    printf("(Jogador %d) A jogada da vez eh: %s - Aposta atual: %d\n", msg.jogador, num2jogada(msg.tipo_jogada), msg.valor_aposta);
    eu.jogada = msg.tipo_jogada;
    printf("deseja apostar?[s/n] ");
    char char_aposta;
    scanf(" %c", &char_aposta);
    //printf("char_aposta = '%c'", char_aposta);
    //printf("jogador: %d\n", eu.num_jogador);

    int minha_aposta = 0;
    if (char_aposta == 's' || char_aposta == 'S') {
      printf("apostado!\n");
      minha_aposta = msg.valor_aposta + 1;
      enviar_mensagem(msg.tipo_msg, eu.num_jogador, minha_aposta, msg.tipo_jogada);
    } else {
      enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
    }

    msg = receber_mensagem(); // TIPO_ATUALIZACAO
    enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
    if (msg.jogador != eu.num_jogador) {
      bastao = 0;

      // encaminha a mensagem para o proximo jogador
      enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

      // atualizacao do jogador de dados
      msg = receber_mensagem(); // TIPO_ATUALIZACAO_FICHAS
      printf("jogador %d ganhou/perdeu %d fichas\n", msg.jogador, msg.valor_aposta);
      // TODO: atualizar os dados do jogador msg.jogador
    } else {
      // é para mim a mensagem...
      bastao = 1;

      int ganho = 0;
      if (jogar_jogada()) {
        ganho = 10; // TODO: mudar para o valor correto
      } else {
        ganho = -minha_aposta;
      }

      enviar_mensagem(TIPO_ATUALIZACAO_FICHAS, eu.num_jogador,  ganho, eu.jogada);
    }
  }

  return 0;
}
