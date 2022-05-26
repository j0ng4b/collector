#ifndef COLLECTOR_TELAS_H
#define COLLECTOR_TELAS_H

#include <time.h>
#include "janela.h"
#include "utilitarios.h"

enum telas {
    TELA_INICIAL,
    TELA_MENU,
    TELA_NIVEIS,
    TELA_JOGO,
    TELA_GAMEOVER
};


/** Tela inicial */
struct tela_inicial {
    int mostra_mensagem;
    clock_t temporizador;
};

void tela_inicial_nova(struct tela_inicial *tela);
void tela_inicial_atualiza(struct tela_inicial *tela);
void tela_inicial_desenha(struct tela_inicial *tela);
void tela_inicial_sair(struct tela_inicial *tela);


/** Tela menu */
struct tela_menu {
    struct {
        struct janela sair;
        struct janela informacoes;
    } janela;

    int opcao_selecionada;
};

void tela_menu_nova(struct tela_menu *tela);
void tela_menu_atualiza(struct tela_menu *tela);
void tela_menu_desenha(struct tela_menu *tela);
void tela_menu_sair(struct tela_menu *tela);


/** Tela níveis */
struct tela_niveis {
    int cores_fundo_jogo[LINHAS];
    int nivel_selecionado;
};

void tela_niveis_nova(struct tela_niveis *tela);
void tela_niveis_atualiza(struct tela_niveis *tela);
void tela_niveis_desenha(struct tela_niveis *tela);
void tela_niveis_sair(struct tela_niveis *tela);


/** Tela jogo */
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

    struct {
        struct janela pausa;
        struct janela timer;
        int pre_timer;
        int estava_visivel;
    } janela;

    clock_t temporizador;
};

void tela_jogo_nova(struct tela_jogo *tela);
void tela_jogo_atualiza(struct tela_jogo *tela);
void tela_jogo_desenha(struct tela_jogo *tela);
void tela_jogo_sair(struct tela_jogo *tela);


/** Tela Gameover */
struct tela_gameover {
    int novo_recorde;
    int lendo_novo_recorde;

    struct {
        int posicao;
        int mostrar;
    } cursor;

    clock_t temporizador;
};

void tela_gameover_nova(struct tela_gameover *tela);
void tela_gameover_atualiza(struct tela_gameover *tela);
void tela_gameover_desenha(struct tela_gameover *tela);
void tela_gameover_sair(struct tela_gameover *tela);

#endif /* COLLECTOR_TELAS_H */

