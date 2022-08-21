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

int eu = 0;
jogador jogadores[NUM_MAQ] = {};
int bastao = 0;

int jogar_jogada() {
      executar_jogada(&jogadores[eu - 1]);
      //printar_dados(&eu);
      ordenar_dados(jogadores[eu - 1].dados);
      printf("Dados ordenados: \n");
      printar_dados(&jogadores[eu  - 1]);
      int conseguiu = verifica_jogada(jogadores[eu - 1].jogada, jogadores[eu - 1].dados);
      if(conseguiu) {
        printf("Voce venceu!!!\n");
        return 1;
      }

      printf("Nao foi dessa vez...\n");
      return 0;
}

void reseta_dados(jogador *jogador){
  for(int i = 0; i < NUM_DADOS; i++)
    jogador->dado_bloqueado[i] = 0;
}

void mostrar_se_ganhou(mensagem msg){
  if(msg.valor_aposta > 0)
    printf("(Jogador %d) ganhou %d fichas!!!\n", msg.jogador, msg.valor_aposta);
  else
    printf("(Jogador %d) perdeu %d fichas...\n", msg.jogador, (msg.valor_aposta * -1));
}

void fluxo_bastao() {
  mensagem msg;
  int apostador;
  int valor_aposta;
  int jogada;

  // usuario escolhe a jogada
  mostrar_jogadas();
  scanf("%d", &jogada);
  int ok = verifica_entrada_jogada(jogada);
  while(!ok){
    printf("Jogada inválida! Selecione de 1 a 8: ");
    scanf("%d", &jogada);
    ok = verifica_entrada_jogada(jogada);
  }

  // inicia as apostas
  enviar_mensagem(TIPO_APOSTA, eu, APOSTA_INICIAL, jogada);

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

  if(msg.jogador == eu){
    int sou_maior_apostador = 1;
    printf("Ninguém apostou, você joga!\n");

    char ganho = 0;

    // Limpar vetor de bloqueados
    reseta_dados(&jogadores[eu - 1]);

    if (jogar_jogada()) {
      ganho = valor_premio_jogada(jogadores[eu - 1].jogada);
    } else {
      ganho = -APOSTA_INICIAL;
    }

    jogadores[eu - 1].num_fichas += ganho;
    printf("Você agora possui %d fichas\n", jogadores[eu - 1].num_fichas);

    // manda atualizacao e espera dar a volta
    enviar_mensagem(TIPO_ATUALIZACAO_FICHAS, eu, ganho, msg.tipo_jogada);

    //printf("esperando TIPO_ATUALIZACAO_FICHAS\n");
    msg = receber_mensagem();
    assert(msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);
    //printf("recebi TIPO_ATUALIZACAO_FICHAS\n");

    // manda bastao, o original deve estar esperando.
    //enviar_mensagem(TIPO_VOLTA_JOG, 0, 0, 0);

    // no proximo precisa esperar o bastao ou aposta

    //return;
  } else {
    // passa o pseudo bastao para o apostador
    enviar_mensagem(TIPO_JOGAR, apostador, valor_aposta, jogada);
    //printf("mandei TIPO_JOGAR apostador %d valor_aposta %d jogada %s\n", apostador, valor_aposta, num2jogada(jogada));

    // recebe atualizacao das fichas do apostador
    msg = receber_mensagem();
    assert(msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);

    jogadores[msg.jogador - 1].num_fichas += msg.valor_aposta;
    mostrar_se_ganhou(msg);
    printf("(Jogador %d) agora está com %d fichas!\n", msg.jogador, jogadores[msg.jogador - 1].num_fichas);

    // repassa TIPO_ATUALIZACAO_FICHAS
    enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

    if (jogadores[msg.jogador - 1].num_fichas <= 0) { // meio sus mas é o que dá
        printf("(Jogador %d) perdeu todas as fichas! Fim de jogo!\n", msg.jogador);
        exit(0);
    }

    msg = receber_mensagem();
    assert(msg.tipo_msg == TIPO_VOLTA_JOG); // espera volta de quem ta jogando 
  }

  enviar_mensagem(TIPO_BASTAO, 0, 0, 0);
  bastao = 0;
}

int perguntar_aposta(int apostador, int tipo_jogada, int aposta_atual, int nova_aposta) {
  printf("(Jogador %d) A jogada da vez é: %s - Aposta atual: %d\n", apostador, num2jogada(tipo_jogada), aposta_atual);
  printf("Você possui %d fichas, deseja apostar %d fichas? [s/n] ", jogadores[eu - 1].num_fichas, aposta_atual + 1);
  
  char char_aposta;
  scanf(" %c", &char_aposta);

  return char_aposta == 's' || char_aposta == 'S';
}

void fluxo_nao_bastao() {
  // recebe a mensagem de aposta
  mensagem msg = receber_mensagem();
  assert(msg.tipo_msg == TIPO_APOSTA || msg.tipo_msg == TIPO_BASTAO || msg.tipo_msg == TIPO_VOLTA_JOG);

  if (msg.tipo_msg == TIPO_BASTAO) {
    // caso seja bastao reinicia o loop como bastao
    //printf("Recebi o bastão!\n");
    bastao = 1;
    return;
  } else if (msg.tipo_msg == TIPO_VOLTA_JOG) {
    // repassa a mensagem para voltar para o bastao antigo
    enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

    msg = receber_mensagem();
    assert(msg.tipo_msg == TIPO_APOSTA);
  }

  // usuario escolhe se quer apostar
  jogadores[eu - 1].jogada = msg.tipo_jogada;

  int minha_aposta = msg.valor_aposta + 1;
  if (perguntar_aposta(msg.jogador, msg.tipo_jogada, msg.valor_aposta, minha_aposta)) {
    printf("Você apostou %d fichas!\n", minha_aposta);
    // encaminha minha aposta
    enviar_mensagem(msg.tipo_msg, eu, minha_aposta, msg.tipo_jogada);
  } else {
    minha_aposta = 0;
    // encaminha aposta
    enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);
  }

  msg = receber_mensagem();
  assert(msg.tipo_msg == TIPO_ATUALIZACAO);
  printf("A maior aposta foi do Jogador %d com %d fichas na jogada %s\n", msg.jogador, msg.valor_aposta, num2jogada(msg.tipo_jogada));

  // reenvia a atualizacao
  enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

  // Essa pode ser tanto TIPO_JOGAR quanto TIPO_ATUALIZACAO_FICHAS,
  // se é ou está antes do jogador com maior aposta, vai ser TIPO_JOGAR
  // se está depois, vai ser TIPO_ATUALIZACAO_FICHAS
  msg = receber_mensagem(); 
  assert(msg.tipo_msg == TIPO_JOGAR || msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);

  int sou_maior_apostador = 0;

  if (msg.tipo_msg == TIPO_JOGAR && msg.jogador == eu) {
      // sou o maior apostador, preciso:
      //  - jogar dados
      //  - mandar atualizacao de fichas
      //  - receber atualizacao de fichas
      //  - mandar bastao para o original

      sou_maior_apostador = 1;
      printf("Você foi quem mais apostou! Você joga!\n");

      char ganho = 0;

      reseta_dados(&jogadores[eu - 1]);

      if (jogar_jogada()) {
        ganho = valor_premio_jogada(jogadores[eu - 1].jogada);
      } else {
        ganho = -minha_aposta;
      }

      jogadores[eu - 1].num_fichas += ganho;

      if(ganho > 0)
        printf("Você ganhou %d fichas e agora possui %d fichas!\n", ganho, jogadores[eu - 1].num_fichas);
      else
        printf("Você perdeu %d fichas e agora possui %d fichas!\n", -ganho, jogadores[eu - 1].num_fichas);


      // manda atualizacao e espera dar a volta
      enviar_mensagem(TIPO_ATUALIZACAO_FICHAS, eu, ganho, msg.tipo_jogada);

      //printf("esperando TIPO_ATUALIZACAO_FICHAS\n");
      msg = receber_mensagem();
      assert(msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);
      //printf("recebi TIPO_ATUALIZACAO_FICHAS\n");

      // manda bastao, o original deve estar esperando.
      enviar_mensagem(TIPO_VOLTA_JOG, 0, 0, 0);

      // no proximo precisa esperar o bastao ou aposta

      return;
  }

  if (msg.tipo_msg == TIPO_JOGAR && msg.jogador != eu) {
    // nao sou, preciso repassar o TIPO_JOGAR e esperar a TIPO_ATUALIZACAO_FICHA
    printf("(Jogador %d) apostou mais que você!\n", msg.jogador);
    enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

    msg = receber_mensagem();
    assert(msg.tipo_msg == TIPO_ATUALIZACAO_FICHAS);
  }

  // aqui msg é TIPO_ATUALIZACAO FICHAS
  jogadores[msg.jogador - 1].num_fichas += msg.valor_aposta;
  mostrar_se_ganhou(msg);
  printf("(Jogador %d) agora está com %d fichas!\n", msg.jogador, jogadores[msg.jogador - 1].num_fichas);

  // jogadores[msg.jogador - 1].fichas -= msg.valor_aposta;

  // repassa TIPO_ATUALIZACAO_FICHAS
  enviar_mensagem(msg.tipo_msg, msg.jogador, msg.valor_aposta, msg.tipo_jogada);

  // no proximo precisa esperar bastao ou aposta
  return;
}

int main(int argc, char *argv[]) {

  if (argc < 6) {
    printf("Modo de uso: %s <porta1> <porta2> <porta3> <porta4> <numero jogador>\n", argv[0]);
    return 1;
  }

  eu = atoi(argv[argc - 1]);
  if(eu < 0 || eu > NUM_MAQ){
    printf("Opção inválida, tente de 1..4!\n");
    exit(0);
  }
  printf("Você é o o Jogador %d!\n", eu);

  // Seleciona as portas

  for(int i = 0; i < NUM_MAQ; i++){
    if(atoi(argv[i + 1]) < 0 || atoi(argv[i + 1]) > 64000){
      printf("Opção inválida, tente portas entre 0..64k!\n");
      exit(0);
    }
    jogadores[i].porta_saida = atoi(argv[i + 1]);
  }
    
  jogadores[0].porta_entrada = jogadores[NUM_MAQ - 1].porta_saida;
  for(int i = 0; i < NUM_MAQ - 1;i++)
    jogadores[i + 1].porta_entrada = jogadores[i].porta_saida;

  for (int i = 0; i < NUM_MAQ; i++) {
    jogadores[i].num_fichas = NUM_FICHAS_INICIAL;
  }

  //printf("PORTA DE ENTRADA: %ld\n", jogadores[eu - 1].porta_entrada);
  //printf("PORTA DE SAIDA: %ld\n", jogadores[eu - 1].porta_saida);

  // Cria o soquete
  inicializa_soquete(jogadores[eu - 1].porta_saida, jogadores[eu - 1].porta_entrada, "127.0.0.1");

  // checa bastao
  char char_bastao;
  printf("Você começa? [s/n] ");
  char_bastao = getchar();
  if (char_bastao == '\n')
    char_bastao = getchar();

  if (char_bastao == 's' || char_bastao == 'S') {
    bastao = 1;
  } else {
    bastao = 0;
  }

  while (1) {
    for (int i = 0; i < NUM_MAQ; i++) {
      if (jogadores[i].num_fichas <= 0) {
        if (i == eu - 1)
          printf("Fim de jogo! Você perdeu...\n");
        else 
          printf("(Jogador %d) perdeu todas as fichas! Fim de jogo!\n", i + 1);
        return 0;
      }
    }

    if (bastao) {
      //printf("Estou com o bastão!\n");
      fluxo_bastao();
    } else {
      //printf("Não estou com o bastão!\n");
      fluxo_nao_bastao();
    }
  }

  return 0;
}
