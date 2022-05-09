#include "utilitarios.h"

#include <time.h>
#include <conio2.h>

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
                cprintf("─");
            } else if (moldura_x == 0 || moldura_x == largura - 1) {
                gotoxy(x + moldura_x, y + moldura_y);
                cprintf("│");
            } else if (!transparente) {
                gotoxy(x + moldura_x, y + moldura_y);
                cprintf(" ");
            }
        }
    }

    gotoxy(x, y);
    cprintf("┌");

    gotoxy(x + largura - 1, y);
    cprintf("┐");

    gotoxy(x, y + altura - 1);
    cprintf("└");

    gotoxy(x + largura - 1, y + altura - 1);
    cprintf("┘");
}

