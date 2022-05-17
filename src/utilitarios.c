#include <time.h>
#include <conio2.h>
#include "utilitarios.h"
#include "collector.h"

void msleep(int msec)
{
    /* A função clock retorna o tempo aproximado do processador usado pelo
     * processo desde o seu início. Esse tempo é convertido em segundos se
     * dividido por CLOCKS_PER_SEC.
     */
    clock_t start = clock();

    /* Parecido com o campo da física com delta T, mas nesse caso verifica se
     * delta T passou de um determinado limite de tempo em segundos, caso
     * não tenha passado espera até que passe.
     */
    while ((clock() - start) / (double) CLOCKS_PER_SEC < msec / 1000.0);
}

void desenha_moldura(int transparente, int x, int y, int largura, int altura)
{
    int moldura_x, moldura_y;

    for (moldura_y = 0; moldura_y < altura; ++moldura_y) {
        for (moldura_x = 0; moldura_x < largura; ++moldura_x) {
            if (moldura_y == 0 || moldura_y == altura - 1) {
                gotoxy(x + moldura_x, y + moldura_y);
#ifdef UNICODE
                cprintf("─");
#else
                cprintf("-");
#endif
            } else if (moldura_x == 0 || moldura_x == largura - 1) {
                gotoxy(x + moldura_x, y + moldura_y);
                cprintf("|");
            } else if (!transparente) {
                gotoxy(x + moldura_x, y + moldura_y);
                cprintf(" ");
            }
        }
    }

    gotoxy(x, y);
#ifdef UNICODE
    cprintf("╭");
#else
    cprintf("+");
#endif

    gotoxy(x + largura - 1, y);
#ifdef UNICODE
    cprintf("╮");
#else
    cprintf("+");
#endif

    gotoxy(x, y + altura - 1);
#ifdef UNICODE
    cprintf("╰");
#else
    cprintf("+");
#endif

    gotoxy(x + largura - 1, y + altura - 1);
#ifdef UNICODE
    cprintf("╯");
#else
    cprintf("+");
#endif
}

void desenha_nome_jogo(int x, int y)
{
    gotoxy(x, y++);
    cprintf("_______ _______ ___     ___     _______ _______ _______ _______ _______");
    gotoxy(x, y++);
    cprintf("|   _   |   _   |   |   |   |   |   _   |   _   |       |   _   |   _   \\");
    gotoxy(x, y++);
    cprintf("|.  1___|.  |   |.  |   |.  |   |.  1___|.  1___|.|   | |.  |   |.  l   /");
    gotoxy(x, y++);
    cprintf("|.  |___|.  |   |.  |___|.  |___|.  __)_|.  |___`-|.  |-|.  |   |.  _   1");
    gotoxy(x, y++);
    cprintf("|:  1   |:  1   |:  1   |:  1   |:  1   |:  1   | |:  | |:  1   |:  |   |");
    gotoxy(x, y++);
    cprintf("|::.. . |::.. . |::.. . |::.. . |::.. . |::.. . | |::.| |::.. . |::.|:. |");
    gotoxy(x, y++);
    cprintf("`-------`-------`-------`-------`-------`-------' `---' `-------`--- ---'");
}

void desenha_linha_recorde(int x, int y, char nome[], int pontos, int posicao)
{
    char linha_potilhada[RECORDE_MAXIMO_TEXTO_LINHA] = { 0 };

    memset(linha_potilhada, '.', sizeof(linha_potilhada) - 1);

    gotoxy(x, y);
    cprintf("%s", linha_potilhada);

    gotoxy(x, y);
    cprintf("%d. ", posicao);
    cprintf("%s ", nome);

    gotoxy(x + RECORDE_MAXIMO_TEXTO_LINHA - 9, y);
    cprintf(" %4d pts", pontos);
}

