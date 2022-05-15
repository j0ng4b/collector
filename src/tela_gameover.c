#include <ctype.h>
#include <time.h>
#include <conio2.h>
#include "telas.h"
#include "collector.h"
#include "utilitarios.h"

#define TAMANHO_MAXIMO_TEXTOS   50
#define NUMERO_LEGENDAS         3
#define VELOCIDADE_PISCA_CURSOR 0.3

static struct tela_gameover reinicia_variaveis(struct tela_gameover tela);
static void desenha_gameover(int x, int y);

struct tela_gameover tela_gameover_nova(void)
{
    struct tela_gameover tela = { 0 };
    return reinicia_variaveis(tela);
}

struct tela_gameover tela_gameover_atualiza(struct tela_gameover tela)
{
    struct contexto contexto = collector_contexto();

    tela.novo_recorde = contexto.novo_recorde;
    if (tela.novo_recorde > 0 && tela.lendo_novo_recorde == 1) {
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;

        if (isalnum(contexto.tecla) || contexto.tecla == ' ') {
            contexto.recordes[tela.novo_recorde - 1]
                .nome[tela.cursor.posicao] = contexto.tecla;

            if (tela.cursor.posicao < RECORDE_TAMANHO_NOME - 2)
                tela.cursor.posicao++;

            contexto.alteracao |= COLLECTOR_CONTEXTO_ALTERAR_RECORDE;
        } else if ((contexto.tecla == 8 || contexto.tecla == 127)
            && tela.cursor.posicao > 0) {
            tela.cursor.posicao--;
        } else if (contexto.tecla == '\r'
            && tela.cursor.posicao < RECORDE_TAMANHO_NOME - 1) {
            tela.cursor.posicao++;
        } else {
            contexto.alteracao = COLLECTOR_CONTEXTO_SEM_ALTERACAO;
        }

        if (tela.cursor.posicao >= RECORDE_TAMANHO_NOME - 1)
            tela.lendo_novo_recorde = 2;
    }

    if ((clock() - tela.temporizador) / (double) CLOCKS_PER_SEC >
        VELOCIDADE_PISCA_CURSOR) {
        tela.cursor.mostrar = !tela.cursor.mostrar;
        tela.temporizador = clock();

        contexto.alteracao |= COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    }

    if (contexto.tecla == 'b' && tela.lendo_novo_recorde == 2) {
        tela = reinicia_variaveis(tela);

        contexto.tela = TELA_NIVEIS;
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;
    }

    collector_altera_contexto(contexto);
    return tela;
}

struct tela_gameover tela_gameover_desenha(struct tela_gameover tela)
{
    struct contexto contexto = collector_contexto();

    char texto_legenda[NUMERO_LEGENDAS][TAMANHO_MAXIMO_TEXTOS] = {
        "enter: confirmar letra/nome",
        "backpace: volta para letra anterior",
        "Digite as letras do seu nome ou espaços",
    };

    int i, j, y;

    y = METADE_LINHAS - 5;
    if (contexto.novo_recorde)
        y -= 4;

    switch (tela.lendo_novo_recorde) {
    case 0:
        textcolor(RED);
        desenha_gameover(METADE_COLUNAS - 32, y);

        y += 8;
        for (i = j = 1; contexto.pontos / j; ++i, j *= 10);

        textcolor(WHITE);
        if (tela.novo_recorde) {
            gotoxy(METADE_COLUNAS - (19 + i) / 2, y);
            cprintf("Novo recorde: %d pts!", contexto.pontos);

            y += 3;
            gotoxy(METADE_COLUNAS - 4, y++);
            cprintf("Recordes");

            i = METADE_COLUNAS - RECORDE_MAXIMO_TEXTO_LINHA / 2;
            for (j = 0; j < 3; ++j)
                desenha_linha_recorde(i, y + j, contexto.recordes[j].nome,
                    contexto.recordes[j].pontos, j + 1);

            tela.lendo_novo_recorde = 1;
        } else {
            gotoxy(METADE_COLUNAS - (17 + i) / 2, y);
            cprintf("Seus pontos: %d pts", contexto.pontos);

            tela.lendo_novo_recorde = 2;
        }

        break;

    case 1:
        textcolor(WHITE);

        i = METADE_COLUNAS - RECORDE_MAXIMO_TEXTO_LINHA / 2 + 3;
        j = METADE_LINHAS + tela.novo_recorde + 2;

        gotoxy(i, j);
        cprintf("%s", contexto.recordes[tela.novo_recorde - 1].nome);

        gotoxy(i + tela.cursor.posicao, j);
        if (tela.cursor.mostrar)
            cprintf("_");

        for (i = 0; i < NUMERO_LEGENDAS; ++i) {
            gotoxy(1, LINHAS - i);
            cprintf("%s", texto_legenda[i]);
        }

        break;

    case 2:
        y += 11;
        if (contexto.novo_recorde)
            y += 6;

        textcolor(WHITE);
        gotoxy(METADE_COLUNAS - 15, y);
        cprintf("Precione b para voltar ao menu");

        for (i = 0; i < NUMERO_LEGENDAS; ++i) {
            gotoxy(1, LINHAS - i);
            clreol();
        }

        break;
    }

    return tela;
}

static struct tela_gameover reinicia_variaveis(struct tela_gameover tela)
{
    tela.novo_recorde = 0;
    tela.lendo_novo_recorde = 0;

    tela.cursor.posicao = 0;
    tela.cursor.mostrar = 1;

    tela.temporizador = clock();
    return tela;
}

static void desenha_gameover(int x, int y)
{
    gotoxy(x, y++);
    cprintf(" _______ _______ ___ ___ _______  _______ ___ ___ _______ _______ ");
    gotoxy(x, y++);
    cprintf("|   _   |   _   |   Y   |   _   ||   _   |   Y   |   _   |   _   \\");
    gotoxy(x, y++);
    cprintf("|.  |___|.  1   |.      |.  1___||.  |   |.  |   |.  1___|.  l   /");
    gotoxy(x, y++);
    cprintf("|.  |   |.  _   |. \\_/  |.  __)_ |.  |   |.  |   |.  __)_|.  _   1");
    gotoxy(x, y++);
    cprintf("|:  1   |:  |   |:  |   |:  1   ||:  1   |:  1   |:  1   |:  |   |");
    gotoxy(x, y++);
    cprintf("|::.. . |::.|:. |::.|:. |::.. . ||::.. . |\\:.. ./|::.. . |::.|:. |");
    gotoxy(x, y++);
    cprintf("`-------`--- ---`--- ---`-------'`-------' `---' `-------`--- ---'");
}

