#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio2.h>
#include "telas.h"
#include "collector.h"
#include "janela.h"

#define LARGURA_COLETOR      10
#define POSICAO_INICIAL_BOLA 2

static struct tela_jogo reinicia_jogo(struct tela_jogo tela);

struct tela_jogo tela_jogo_nova(void)
{
    struct tela_jogo tela = { 0 };
    struct janela_cores cores_janela = { BLUE, WHITE };

    srand(time(NULL));
    tela = reinicia_jogo(tela);

    tela.janela.pausa = janela_nova(30, 6, "Jogo pausado",
        "Voltar ao menu de niveis?", cores_janela);

    tela.janela.pausa = janela_adiciona_botao(tela.janela.pausa,
        JANELA_BOTAO_ACEITAR, "Sim");

    tela.janela.pausa = janela_adiciona_botao(tela.janela.pausa,
        JANELA_BOTAO_NEGAR, "Nao");

    return tela;
}

struct tela_jogo tela_jogo_atualiza(struct tela_jogo tela)
{
    struct contexto contexto = collector_contexto();
    int janela_botao_selecionado = tela.janela.pausa.botao_selecionado;

    if (tolower(contexto.tecla) == 'p') {
        if (tela.janela.pausa.visivel) {
            tela.janela.pausa.visivel = 0;
            tela.janela.estava_visivel = 1;
        } else {
            tela.janela.pausa = janela_mostrar(tela.janela.pausa);
        }

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    }

    if (tela.janela.pausa.visivel) {
        tela.janela.pausa = janela_atualiza(tela.janela.pausa, contexto.tecla);

        if (janela_botao_selecionado != tela.janela.pausa.botao_selecionado) {
            tela.janela.estava_visivel = 1;
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
        }

        switch (tela.janela.pausa.botao_clicado) {
        case JANELA_BOTAO_ACEITAR:
            tela = reinicia_jogo(tela);

            contexto.tela = TELA_NIVEIS;
            contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;
            break;

        default:
            break;
        }

        collector_altera_contexto(contexto);
        return tela;
    } else if (contexto.tecla == 'd' &&
        tela.coletor.posicao_x + LARGURA_COLETOR <= COLUNAS) {
        tela.coletor.posicao_x++;

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    } else if (contexto.tecla == 'a' && tela.coletor.posicao_x > 1) {
        tela.coletor.posicao_x--;

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    }

    if ((clock() - tela.temporizador) / (double) CLOCKS_PER_SEC >
        tela.bola.velocidade_y) {
        tela.temporizador = clock();
        tela.bola.posicao_y++;

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    }

    switch (tela.coletor.pontos) {
    case 0:
        tela.bola.velocidade_y = 0.3 / (contexto.nivel_dificuldade + 1);
        break;

    case 5:
        tela.bola.velocidade_y = 0.2 / (contexto.nivel_dificuldade + 1);
        break;

    case 10:
        tela.bola.velocidade_y = 0.1 / (contexto.nivel_dificuldade + 1);
        break;

    case 30:
        tela.bola.velocidade_y = 0.005 / (contexto.nivel_dificuldade + 1);
        break;

    case 40:
        tela.bola.velocidade_y = 0.003 / (contexto.nivel_dificuldade + 1);
        break;
    }

    if (tela.bola.posicao_y == tela.coletor.posicao_y) {
        if (tela.bola.posicao_x >= tela.coletor.posicao_x
            && tela.bola.posicao_x < tela.coletor.posicao_x
                + LARGURA_COLETOR) {
            tela.bola.posicao_x = 1 + rand() % COLUNAS;
            tela.bola.posicao_y = POSICAO_INICIAL_BOLA;

            tela.coletor.pontos++;
        } else {
            contexto.pontos = tela.coletor.pontos;
            tela = reinicia_jogo(tela);

            contexto.tela = TELA_GAMEOVER;
            contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA |
                COLLECTOR_CONTEXTO_ALTERAR_PONTOS;
        }
    }

    collector_altera_contexto(contexto);
    return tela;
}

struct tela_jogo tela_jogo_desenha(struct tela_jogo tela)
{
    char linha_vazia[COLUNAS + 1];
    int i;

    if (tela.janela.pausa.visivel) {
        textcolor(BLACK);
        textbackground(WHITE);
        gotoxy(COLUNAS - 6, 1);
        cprintf(" |> ");

        tela.janela.pausa = janela_desenha(tela.janela.pausa);
        return tela;
    }

    memset(linha_vazia, ' ', sizeof(linha_vazia));
    linha_vazia[COLUNAS] = '\0';

    if (tela.janela.estava_visivel) {
        textbackground(BLUE);
        for (i = 0; i < tela.janela.pausa.altura; ++i) {
            gotoxy(tela.janela.pausa.x, tela.janela.pausa.y + i);
            cprintf("%.*s", tela.janela.pausa.largura, linha_vazia);
        }

        tela.janela.estava_visivel = 0;
    }

    textcolor(BLACK);
    textbackground(BLUE);

    gotoxy(1, 1);
    cprintf("Pontos: %d", tela.coletor.pontos);

    textcolor(BLACK);
    textbackground(WHITE);
    gotoxy(COLUNAS - 6, 1);
    cprintf(" || ");

    textcolor(WHITE);
    textbackground(BLACK);
    cprintf(" P ");


    for (i = 0; i < 2; ++i) {
        textbackground(BLUE);
        gotoxy(1, tela.coletor.posicao_y - (1 - i));
        cprintf("%s", linha_vazia);

        textbackground(i == 0 ? YELLOW : BLUE);
        gotoxy(tela.bola.posicao_x, tela.bola.posicao_y - i);
        cprintf(" ");
    }

    textbackground(WHITE);
    for (i = 0; i < LARGURA_COLETOR; ++i) {
        gotoxy(tela.coletor.posicao_x + i, tela.coletor.posicao_y);
        cprintf(" ");
    }

    return tela;
}

/* reinicia_jogo
 *
 * Função para reiniciar as variáveis do jogo tanto em caso de game over quanto
 * em caso de ser a primeira execução do jogo
 */
struct tela_jogo reinicia_jogo(struct tela_jogo tela)
{
    tela.coletor.posicao_x = METADE_COLUNAS - LARGURA_COLETOR / 2;
    tela.coletor.posicao_y = LINHAS * 0.8;
    tela.coletor.pontos = 0;

    tela.bola.posicao_x = 1 + rand() % COLUNAS;
    tela.bola.posicao_y = POSICAO_INICIAL_BOLA;
    tela.bola.velocidade_y = 0.3;

    return tela;
}

