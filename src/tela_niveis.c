#include <conio2.h>
#include "telas.h"
#include "collector.h"

#define TAMANHO_MAXIMO_TEXTOS     50
#define NUMERO_NIVEIS_DIFICULDADE 3
#define NUMERO_LEGENDAS           3

struct tela_niveis tela_niveis_nova(void)
{
    struct tela_niveis tela = { 0 };

    tela.nivel_selecionado = 0;
    return tela;
}

struct tela_niveis tela_niveis_atualiza(struct tela_niveis tela)
{
    struct contexto contexto = collector_contexto();

    if (contexto.tecla == 'a' && tela.nivel_selecionado > 0) {
        tela.nivel_selecionado--;
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    } else if (contexto.tecla == 'd'
        && tela.nivel_selecionado < NUMERO_NIVEIS_DIFICULDADE - 1) {
        tela.nivel_selecionado++;
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    } else if (contexto.tecla == 'b') {
        contexto.tela = TELA_MENU;
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;
    } else if (contexto.tecla == '\r') {
        contexto.tela = TELA_JOGO;
        contexto.nivel_dificuldade = tela.nivel_selecionado;
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA |
            COLLECTOR_CONTEXTO_ALTERAR_NIVEL;
    }

    collector_altera_contexto(contexto);
    return tela;
}

struct tela_niveis tela_niveis_desenha(struct tela_niveis tela)
{
    char texto_dificuldades[NUMERO_NIVEIS_DIFICULDADE][TAMANHO_MAXIMO_TEXTOS] = {
        "Facil", "Medio", "Dificil",
    };

    char texto_legendas[NUMERO_LEGENDAS][TAMANHO_MAXIMO_TEXTOS] = {
        "enter: confirma o nivel selecionado",
        "b: volta para o menu",
        "a/d: seleciona o nivel inicial"
    };

    int i;
    int j = METADE_LINHAS - 4;

    textcolor(WHITE);
    clrscr();

    gotoxy(METADE_COLUNAS - 7, j++);
    cprintf("Nivel inicial");

    for (i = 0; i < NUMERO_NIVEIS_DIFICULDADE; ++i) {
        if (tela.nivel_selecionado == i)
            textcolor(RED);
        else
            textcolor(WHITE);

        cprintf("%s", texto_dificuldades[i]);
    }

    gotoxy(METADE_COLUNAS - 11, ++j);
    gotoxy(METADE_COLUNAS - 3, j);
    gotoxy(METADE_COLUNAS + 5, j++);

    j += 2;

    /*
    textcolor(WHITE);

    gotoxy(METADE_COLUNAS - 4, j++);
    cprintf("Recordes");

    for (k = 0; k < 3; ++k, ++j) {
        gotoxy(METADE_COLUNAS - RECORDE_MAXIMO_TEXTO_LINHA / 2, j);

        for (i = 0; i < RECORDE_MAXIMO_TEXTO_LINHA; ++i) {
            if (i == 0 && (i += 3))
                cprintf("%d. ", k + 1);
            else if (i >= 4 && i - 4 < TAMANHO_NOME_RECORDE)
                cprintf("%c", recorde_nomes[k][i - 4]);
            else if (i - 4 == TAMANHO_NOME_RECORDE)
                cprintf(" ");
            else if (i == RECORDE_MAXIMO_TEXTO_LINHA - 9 && (i += 9))
                cprintf(" %4d pts", recorde_maximos[k]);
            else
                cprintf(".");
        }
    }
    */

    textcolor(WHITE);
    for (i = 0; i < NUMERO_LEGENDAS; ++i) {
        gotoxy(1, LINHAS - i);
        cprintf("%s", texto_legendas[i]);
    }

    return tela;
}

