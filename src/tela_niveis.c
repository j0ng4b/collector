#include <strings.h>
#include <conio2.h>
#include "telas.h"
#include "collector.h"

#define TAMANHO_MAXIMO_TEXTOS     50
#define NUMERO_NIVEIS_DIFICULDADE 3
#define NUMERO_LEGENDAS           3

void tela_niveis_nova(struct tela_niveis *tela)
{
    int i;

    for (i = 0; i < LINHAS; ++i)
        if (i < LINHAS * 0.8)
            tela->cores_fundo_jogo[i] = BLUE;
        else
            tela->cores_fundo_jogo[i] = GREEN;

    tela->nivel_selecionado = 0;
}

void tela_niveis_atualiza(struct tela_niveis *tela)
{
    struct contexto contexto = collector_contexto();

    if (contexto.tecla == 'a' && tela->nivel_selecionado > 0) {
        tela->nivel_selecionado--;

        contexto.numero_audio = 0;
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA
            | COLLECTOR_CONTEXTO_TOCAR_AUDIO;
    } else if (contexto.tecla == 'd'
        && tela->nivel_selecionado < NUMERO_NIVEIS_DIFICULDADE - 1) {
        tela->nivel_selecionado++;

        contexto.numero_audio = 0;
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA
            | COLLECTOR_CONTEXTO_TOCAR_AUDIO;
    } else if (contexto.tecla == 'b') {
        contexto.tela = TELA_MENU;
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;

    } else if (contexto.tecla == '\r') {
        contexto.tela = TELA_JOGO;
        contexto.nivel_dificuldade = tela->nivel_selecionado;
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA |
            COLLECTOR_CONTEXTO_ALTERAR_NIVEL |
            COLLECTOR_CONTEXTO_CORES_FUNDO_ANIMACAO;

        memcpy(contexto.cores_fundo_animacao_normal, tela->cores_fundo_jogo,
            sizeof(tela->cores_fundo_jogo));
    }

    collector_altera_contexto(&contexto);
}

void tela_niveis_desenha(struct tela_niveis *tela)
{
    struct contexto contexto = collector_contexto();

    char texto_niveis[NUMERO_NIVEIS_DIFICULDADE][TAMANHO_MAXIMO_TEXTOS] = {
        "Facil", "Medio", "Dificil",
    };

    char texto_legendas[NUMERO_LEGENDAS][TAMANHO_MAXIMO_TEXTOS] = {
        "enter: confirma o nivel selecionado",
        "b: volta para o menu",
        "a/d: seleciona o nivel inicial"
    };

    int tamenho_texto_niveis = 0;

    int i, j;
    int y = METADE_LINHAS - 4;

    clrscr();

    for (i = 0; i < NUMERO_NIVEIS_DIFICULDADE; ++i)
        tamenho_texto_niveis += strlen(texto_niveis[i]) + 2;
    tamenho_texto_niveis /= 2;

    textbackground(RED);
    gotoxy(METADE_COLUNAS - 7, y++);
    cprintf(" Nivel inicial ");

    y++;
    for (i = 0; i < NUMERO_NIVEIS_DIFICULDADE; ++i) {
        gotoxy(METADE_COLUNAS - tamenho_texto_niveis, y++);

        if (tela->nivel_selecionado == i)
            textbackground(RED);
        else
            textbackground(BLACK);

        cprintf(" %s ", texto_niveis[i]);
        tamenho_texto_niveis -= strlen(texto_niveis[i]) + 2;
    }

    y += 2;
    textbackground(BLACK);
    gotoxy(METADE_COLUNAS - 4, y++);
    cprintf("Recordes");

    i = METADE_COLUNAS - RECORDE_MAXIMO_TEXTO_LINHA / 2;
    for (j = 0; j < 3; ++j)
        desenha_linha_recorde(i, y + j, contexto.recordes[j].nome,
            contexto.recordes[j].pontos, j + 1);

    textcolor(WHITE);
    for (i = 0; i < NUMERO_LEGENDAS; ++i) {
        gotoxy(1, LINHAS - i);
        cprintf("%s", texto_legendas[i]);
    }
}

