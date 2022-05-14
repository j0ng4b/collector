#ifndef COLLECTOR_TELAS_H
#define COLLECTOR_TELAS_H

#include <time.h>
#include "collector.h"

enum telas {
    TELA_INICIAL,
    TELA_MENU,
    TELA_JOGO,
    TELA_GAMEOVER,
    TELA_DIFICULDADES,
};


struct tela_inicial {
    int mostra_mensagem;
    clock_t temporizador;
};

struct tela_inicial nova_tela_inicial(void);
struct tela_inicial atualiza_tela_inicial(struct tela_inicial tela_inicial,
    int tecla);
struct tela_inicial desenha_tela_inicial(struct tela_inicial tela_inicial);


struct tela_menu {
    int opcao_selecionada;
    struct janela janela_sair;
    struct janela janela_informacoes;
};

struct tela_menu nova_tela_menu(void);
struct tela_menu atualiza_tela_menu(struct tela_menu tela_menu, int tecla);
struct tela_menu desenha_tela_menu(struct tela_menu tela_menu);


void atualiza_tela_dificuldades(int tecla);
void desenha_tela_dificuldades(void);


enum tela_jogo_dificuldades {
    TELA_JOGO_DIFICULDADE_FACIL,
    TELA_JOGO_DIFICULDADE_MEDIA,
    TELA_JOGO_DIFICULDADE_DIFICIL,
};

struct tela_jogo {
    struct {
        int posicao_x;
        int posicao_y;

        int pontos;
    } coletor;

    struct {
        int posicao_x;
        int posicao_y;

        float velocidade_y;
    } bola;

    enum tela_jogo_dificuldades dificuldade;
    clock_t temporizador;
};


struct tela_jogo nova_tela_jogo(void);
struct tela_jogo atualiza_tela_jogo(struct tela_jogo tela_jogo, int tecla);
struct tela_jogo desenha_tela_jogo(struct tela_jogo tela_jogo);
void tela_jogo_muda_dificuldade(void);


void atualiza_tela_gameover(void);
void desenha_tela_gameover(void);

#endif /* COLLECTOR_TELAS_H */

