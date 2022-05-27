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

static void reinicia_jogo(struct tela_jogo *tela);

void tela_jogo_nova(struct tela_jogo *tela)
{
    struct janela_config janela_pausa_config = {
        { 30, 6 },
        {
            "Jogo pausado",
            "Voltar ao menu de niveis?",

            0
        }, 0,
        { BLUE, WHITE },
        JANELA_COMUM
    };

    struct janela_config janela_timer_config = {
        { 0 },
        { 0 },
        5,
        { BLUE, WHITE },
        JANELA_TEMPORIZADORA
    };

    srand(time(NULL));
    janela_nova(&tela->janela.pausa, &janela_pausa_config);

    janela_adiciona_botao(&tela->janela.pausa, JANELA_BOTAO_ACEITAR, "Sim");
    janela_adiciona_botao(&tela->janela.pausa, JANELA_BOTAO_NEGAR, "Nao");

    janela_nova(&tela->janela.timer, &janela_timer_config);

    reinicia_jogo(tela);
}

void tela_jogo_atualiza(struct tela_jogo *tela)
{
    int i;
    struct contexto contexto = collector_contexto();

    if (tolower(contexto.tecla) == 'p' && !tela->janela.timer.visivel) {
        if (tela->janela.pausa.visivel) {
            tela->janela.pausa.visivel = 0;
            tela->janela.estava_visivel = 1;
        } else {
            janela_mostrar(&tela->janela.pausa);
        }

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    }

    if (tela->janela.pausa.visivel) {
        if (tela->janela.pausa.redesenhar) {
            contexto.numero_audio = 0;
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA
                | COLLECTOR_CONTEXTO_TOCAR_AUDIO;
        }

        tela->janela.estava_visivel = 1;
        switch (janela_atualiza(&tela->janela.pausa, contexto.tecla)) {
        case JANELA_BOTAO_ACEITAR:
            reinicia_jogo(tela);

            contexto.tela = TELA_NIVEIS;
            contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;
            break;

        default:
            break;
        }
        tela->janela.estava_visivel ^= tela->janela.pausa.visivel;

        collector_altera_contexto(&contexto);
        return;
    } else if (tela->janela.timer.visivel) {
        tela->janela.estava_visivel = 1;
        janela_atualiza(&tela->janela.timer, contexto.tecla);
        tela->janela.estava_visivel ^= tela->janela.timer.visivel;

        if (tela->janela.timer.redesenhar)
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;

        collector_altera_contexto(&contexto);
        return;
    } else if (tolower(contexto.tecla)  == 'd') {
        tela->coletor.posicao_x++;
        if( tela->coletor.posicao_x >=COLUNAS)
            tela->coletor.posicao_x=1;

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    } else if (tolower(contexto.tecla) == 'a' ) {
        tela->coletor.posicao_x--;
        if( tela->coletor.posicao_x < 1)
            tela->coletor.posicao_x=COLUNAS-1;

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    }


    if ((clock() - tela->temporizador) / (double) CLOCKS_PER_SEC >
        tela->bola.velocidade_y) {
        tela->temporizador = clock();
        tela->bola.posicao_y++;

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    }

    switch (tela->coletor.pontos) {
    case 0:
        tela->bola.velocidade_y = 0.3 / (contexto.nivel_dificuldade + 1);
        break;

    case 5:
        tela->bola.velocidade_y = 0.2 / (contexto.nivel_dificuldade + 1);
        break;

    case 10:
        tela->bola.velocidade_y = 0.1 / (contexto.nivel_dificuldade + 1);
        break;

    case 30:
        tela->bola.velocidade_y = 0.005 / (contexto.nivel_dificuldade + 1);
        break;

    case 40:
        tela->bola.velocidade_y = 0.003 / (contexto.nivel_dificuldade + 1);
        break;
    }

    if (tela->bola.posicao_y >= tela->coletor.posicao_y) {
        if (tela->bola.posicao_y == tela->coletor.posicao_y){
             for (i = 0; i < LARGURA_COLETOR; ++i) {
        if(((tela->coletor.posicao_x + i)%COLUNAS)==tela->bola.posicao_x){
            tela->bola.posicao_x = 1 + rand() % COLUNAS;
            tela->bola.posicao_y = POSICAO_INICIAL_BOLA;


            tela->coletor.pontos++;}
             }
        } else if (tela->bola.posicao_y - 2 == tela->coletor.posicao_y) {
            contexto.pontos = tela->coletor.pontos;
            reinicia_jogo(tela);

            contexto.tela = TELA_GAMEOVER;
            contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA |
                COLLECTOR_CONTEXTO_ALTERAR_PONTOS;
        }
    }

    collector_altera_contexto(&contexto);
}

void tela_jogo_desenha(struct tela_jogo *tela)
{
    char linha_vazia[COLUNAS + 1];
    int i;

    if (tela->janela.pausa.visivel) {
        textcolor(BLACK);
        textbackground(WHITE);
        gotoxy(COLUNAS - 6, 1);
        cprintf(" |> ");

        janela_desenha(&tela->janela.pausa);
        return;
    } else if (tela->janela.timer.visivel) {
        janela_desenha(&tela->janela.timer);

        if (!tela->janela.pre_timer)
            return;
        else
            tela->janela.pre_timer -= tela->janela.pre_timer > 0;
    }

    memset(linha_vazia, ' ', sizeof(linha_vazia));
    linha_vazia[COLUNAS] = '\0';

    if (tela->janela.estava_visivel) {
        textbackground(BLUE);
        for (i = 0; i < tela->janela.pausa.tamanho.altura; ++i) {
            gotoxy(tela->janela.pausa.posicao.x, tela->janela.pausa.posicao.y + i);
            cprintf("%.*s", tela->janela.pausa.tamanho.largura, linha_vazia);
        }

        tela->janela.estava_visivel = 0;
    }

    textcolor(BLACK);
    textbackground(BLUE);

    gotoxy(1, 1);
    cprintf("Pontos: %d", tela->coletor.pontos);

    textcolor(BLACK);
    textbackground(WHITE);
    gotoxy(COLUNAS - 6, 1);
    cprintf(" || ");

    textcolor(WHITE);
    textbackground(BLACK);
    cprintf(" P ");


    for (i = 0; i < 2; ++i) {
        textbackground(BLUE);
        gotoxy(1, tela->coletor.posicao_y - (1 - i));
        cprintf("%s", linha_vazia);

        if (i == 0 && tela->bola.posicao_y == POSICAO_INICIAL_BOLA)
            continue;

        textbackground(i ? YELLOW : BLUE);
        gotoxy(tela->bola.posicao_x, tela->bola.posicao_y - (1 - i));
        cprintf(" ");
    }

    textbackground(WHITE);
    for (i = 0; i < LARGURA_COLETOR; ++i) {
        gotoxy((tela->coletor.posicao_x + i)%COLUNAS, tela->coletor.posicao_y );
        cprintf(" ");
    }
}

/* reinicia_jogo
 *
 * Função para reiniciar as variáveis do jogo tanto em caso de game over quanto
 * em caso de ser a primeira execução do jogo
 */
static void reinicia_jogo(struct tela_jogo *tela)
{
    tela->coletor.posicao_x = METADE_COLUNAS - LARGURA_COLETOR / 2;
    tela->coletor.posicao_y = LINHAS * 0.8;
    tela->coletor.pontos = 0;

    tela->bola.posicao_x = 1 + rand() % COLUNAS;
    tela->bola.posicao_y = POSICAO_INICIAL_BOLA;
    tela->bola.velocidade_y = 0.3;

    tela->temporizador = clock();

    janela_reiniciar_tempo(&tela->janela.timer);
    tela->janela.pre_timer = 2;
}

