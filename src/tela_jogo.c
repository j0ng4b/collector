#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio2.h>
#include "collector.h"

#define TELA_JOGO_LARGURA_COLETOR      10
#define TELA_JOGO_POSICAO_INICIAL_BOLA 2

static struct tela_jogo reinicia_jogo(struct tela_jogo tela_jogo);

struct tela_jogo nova_tela_jogo(void)
{
    struct tela_jogo tela_jogo = { 0 };

    srand(time(NULL));
    tela_jogo = reinicia_jogo(tela_jogo);

    return tela_jogo;
}

struct tela_jogo atualiza_tela_jogo(struct tela_jogo tela_jogo, int tecla)
{
    struct pedido_colletor pedido = { 0 };

    if (tecla == 'b') {
        pedido.tela = TELA_MENU;
        pedido.pedido = COLLECTOR_MUDAR_TELA;
    } else if (tecla == 'd' &&
        tela_jogo.coletor.posicao_x + TELA_JOGO_LARGURA_COLETOR <= COLUNAS) {
        tela_jogo.coletor.posicao_x++;
        pedido.pedido = COLLECTOR_REDESENHAR_TELA;
    } else if (tecla == 'a' && tela_jogo.coletor.posicao_x > 1) {
        tela_jogo.coletor.posicao_x--;
        pedido.pedido = COLLECTOR_REDESENHAR_TELA;
    }

    if ((clock() - tela_jogo.temporizador) / (double) CLOCKS_PER_SEC >
        tela_jogo.bola.velocidade_y) {
        tela_jogo.temporizador = clock();
        tela_jogo.bola.posicao_y++;
        pedido.pedido = COLLECTOR_REDESENHAR_TELA;
    }

    if (tela_jogo.bola.posicao_y == tela_jogo.coletor.posicao_y) {
        if (tela_jogo.bola.posicao_x >= tela_jogo.coletor.posicao_x
            && tela_jogo.bola.posicao_x < tela_jogo.coletor.posicao_x
                + TELA_JOGO_LARGURA_COLETOR) {
            tela_jogo.bola.posicao_x = 1 + rand() % COLUNAS;
            tela_jogo.bola.posicao_y = TELA_JOGO_POSICAO_INICIAL_BOLA;

            tela_jogo.coletor.pontos++;
        } else {
            tela_jogo = reinicia_jogo(tela_jogo);
            pedido.tela = TELA_GAMEOVER;
            pedido.pedido = COLLECTOR_MUDAR_TELA;
        }
    }

    pedir_collector(pedido);
    return tela_jogo;
}

struct tela_jogo desenha_tela_jogo(struct tela_jogo tela_jogo)
{
    char linha_vazia[COLUNAS + 1];
    int i;

    memset(linha_vazia, ' ', sizeof(linha_vazia));
    linha_vazia[COLUNAS] = '\0';

    textcolor(BLACK);
    textbackground(BLUE);

    gotoxy(1, 1);
    cprintf("Pontos: %d", tela_jogo.coletor.pontos);

    for (i = 0; i < 2; ++i) {
        textbackground(BLUE);
        gotoxy(1, tela_jogo.coletor.posicao_y - (1 - i));
        cprintf("%s", linha_vazia);

        textbackground(i == 0 ? YELLOW : BLUE);
        gotoxy(tela_jogo.bola.posicao_x, tela_jogo.bola.posicao_y - i);
        cprintf(" ");
    }

    textbackground(WHITE);
    for (i = 0; i < TELA_JOGO_LARGURA_COLETOR; ++i) {
        gotoxy(tela_jogo.coletor.posicao_x + i, tela_jogo.coletor.posicao_y);
        cprintf(" ");
    }

    return tela_jogo;
}

/* reinicia_jogo
 *
 * Função para reiniciar as variáveis do jogo tanto em caso de game over quanto
 * em caso de ser a primeira execução do jogo
 */
struct tela_jogo reinicia_jogo(struct tela_jogo tela_jogo)
{
    tela_jogo.coletor.posicao_x = METADE_COLUNAS - TELA_JOGO_LARGURA_COLETOR / 2;
    tela_jogo.coletor.posicao_y = LINHAS * 0.8;
    tela_jogo.coletor.pontos = 0;

    tela_jogo.bola.posicao_x = 1 + rand() % COLUNAS;
    tela_jogo.bola.posicao_y = TELA_JOGO_POSICAO_INICIAL_BOLA;
    tela_jogo.bola.velocidade_y = 0.3;

    return tela_jogo;
}

