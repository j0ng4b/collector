/*
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include <conio2.h>
*/
#include "collector.h"
/*


#define NUMERO_RECORDES               3
#define TAMANHO_NOME_RECORDE          7
#define RECORDE_MAXIMO_TEXTO_LINHA    43

int recorde_nome_letra = 0;
int novo_recorde_index = -1;
int recorde_maximos[NUMERO_RECORDES] = {0};
char recorde_nomes[NUMERO_RECORDES][TAMANHO_NOME_RECORDE] = {
    "FELIPE", "IGOR  ", "J0NG4B",
};
*/
/* animacao_gameover
 *
 * Função para criar uma animação de várias bolinhas caindo.
 */
/*
void animacao_gameover(void)
{
    int i;
    int bolas[COLUNAS + 1] = {0};

    textcolor(WHITE);
    while (bolas[COLUNAS] <= LINHAS) {
        for (i = 1; i <= COLUNAS; ++i) {
            if (bolas[i] > 0) {
                if (bolas[i] <= LINHAS) {
                    textbackground(WHITE);

                    gotoxy(i, bolas[i]);
                    cprintf(" ");

                    gotoxy(COLUNAS + 1 - i, bolas[i]);
                    cprintf(" ");
                }

                if (bolas[i] > 1) {
                    textbackground(BLACK);

                    gotoxy(i, bolas[i] - 1);
                    cprintf(" ");

                    gotoxy(COLUNAS + 1 - i, bolas[i] - 1);
                    cprintf(" ");
                }
            }

            if (i > 1)
                bolas[i] += bolas[i] <= LINHAS && bolas[i - 1] > 3;
            else
                bolas[i] += bolas[i] <= LINHAS;
        }

        gotoxy(COLUNAS / 2, LINHAS);
    }

    textbackground(BLACK);

    gotoxy(1, bolas[COLUNAS] - 1);
    cprintf(" ");

    gotoxy(COLUNAS, bolas[COLUNAS] - 1);
    cprintf(" ");
}
*/

/* novo_recorde
 *
 * Função para verificar se o jogador conseguiu atingir uma nova pontuação
 * máxima. Se ele conseguiu retorna a posição dessa nova pontuação na lista de
 * recordes e atribuí essa nova pontuação na lista de recordes, do contrário
 * apenas retorna -1.
 */
/*
int novo_recorde(void)
{
    int i, j, k;

    for (i = 0; i < NUMERO_RECORDES; ++i) {
        if (recorde_maximos[i] < coletor_pontos) {
            * Move todos os recordes uma posição para baixo. Na posição do novo
             * recorde a nova pontuação é atribuída e o antigo nome é
             * substituído.
             *
            for (j = NUMERO_RECORDES - 1; j >= i; --j) {
                recorde_maximos[j] = j == i ?
                    coletor_pontos : recorde_maximos[j - 1];

                for (k = 0; k < TAMANHO_NOME_RECORDE - 1; ++k)
                    recorde_nomes[j][k] = j == i ?
                        'A' : recorde_nomes[j - 1][k];
            }

            return i;
        }
    }

    return -1;
}

*/

int main(void)
{
    struct collector collector;

    collector = novo_collector();
    roda_collector(collector);

    /*
        if (tela_anterior != tela) {
            tela_anterior = tela;
            redesenhar = 1;

            if (tela == TELA_GAMEOVER) {
                animacao_gameover();
            } else {
                textcolor(WHITE);

                for (i = 1; i <= METADE_LINHAS; ++i) {
                    textbackground(WHITE);

                    gotoxy(1, i);
                    cprintf("%s", linha_caracteres);

                    gotoxy(1, LINHAS - (i - 1));
                    cprintf("%s", linha_caracteres);

                    if (i > 1) {
                        textbackground(clear_color1);

                        gotoxy(1, i - 1);
                        cprintf("%s", linha_caracteres);

                        if (i <= LINHAS * 0.2)
                            textbackground(clear_color2);

                        gotoxy(1, LINHAS - (i - 2));
                        cprintf("%s", linha_caracteres);
                    }

                    gotoxy(METADE_COLUNAS, METADE_LINHAS);
                    msleep(50);
                }

                textbackground(clear_color1);
                for (j = METADE_COLUNAS; j > 0; --j) {
                    for (i = 0; i < 2; ++i) {
                        gotoxy(j, METADE_LINHAS + i);
                        cprintf(" ");

                        gotoxy(COLUNAS - (j - 1), METADE_LINHAS + i);
                        cprintf(" ");
                    }

                    gotoxy(METADE_COLUNAS, LINHAS);
                    msleep(30);
                }
            }

            while (kbhit()) getch();
        }

        } else if (tela == TELA_GAMEOVER) {
            if (novo_recorde_index > -1
                && recorde_nome_letra < TAMANHO_NOME_RECORDE - 1) {
                redesenhar = 2;

                if (isalnum(tecla) || tecla == ' ') {
                    recorde_nomes[novo_recorde_index][recorde_nome_letra] = toupper(tecla);
                    recorde_nome_letra += recorde_nome_letra < TAMANHO_NOME_RECORDE - 2;
                } else if (tecla == 8 || tecla == 127) {
                    recorde_nome_letra -= recorde_nome_letra > 0;
                } else if (tecla == '\r') {
                    recorde_nome_letra += recorde_nome_letra < TAMANHO_NOME_RECORDE - 1;
                } else {
                    redesenhar -= redesenhar == 2 ? 2 : 0;
                }

                if ((clock() - temporizador) / (double) CLOCKS_PER_SEC > 0.3) {
                    temporizador_flag = !temporizador_flag;
                    temporizador = clock();
                    redesenhar = 2;
                }
            } else if (tecla == 'b') {
                tela = TELA_MENU;

                clear_color1 = BLACK;
                clear_color2 = BLACK;

                reinicia_jogo();
            }

            if (redesenhar == 1) {
                i = METADE_COLUNAS - 32;

                if ((novo_recorde_index = novo_recorde()) > -1)
                    j = METADE_LINHAS - 9;
                else
                    j = METADE_LINHAS - 5;

                textcolor(RED);

                gotoxy(i, j++);
                cprintf(" _______ _______ ___ ___ _______  _______ ___ ___ _______ _______ ");
                gotoxy(i, j++);
                cprintf("|   _   |   _   |   Y   |   _   ||   _   |   Y   |   _   |   _   \\");
                gotoxy(i, j++);
                cprintf("|.  |___|.  1   |.      |.  1___||.  |   |.  |   |.  1___|.  l   /");
                gotoxy(i, j++);
                cprintf("|.  |   |.  _   |. \\_/  |.  __)_ |.  |   |.  |   |.  __)_|.  _   1");
                gotoxy(i, j++);
                cprintf("|:  1   |:  |   |:  |   |:  1   ||:  1   |:  1   |:  1   |:  |   |");
                gotoxy(i, j++);
                cprintf("|::.. . |::.|:. |::.|:. |::.. . ||::.. . |\\:.. ./|::.. . |::.|:. |");
                gotoxy(i, j++);
                cprintf("`-------`--- ---`--- ---`-------'`-------' `---' `-------`--- ---'");

                textcolor(WHITE);

                if (novo_recorde_index > -1) {
                    for (i = 1, k = 1; coletor_pontos / k; ++i, k *= 10);

                    gotoxy(METADE_COLUNAS - (19 + i) / 2, ++j);
                    cprintf("Novo recorde: %d pts!", coletor_pontos);

                    j += 3;
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

                    gotoxy(1, LINHAS - 3);
                    cprintf("Digite as letras do seu nome ou espaços");

                    gotoxy(1, LINHAS - 2);
                    cprintf("backpace: volta para letra anterior");

                    gotoxy(1, LINHAS - 1);
                    cprintf("enter: confirmar letra/nome");
                } else {
                    for (i = 1, k = 1; coletor_pontos / k; ++i, k *= 10);

                    gotoxy(METADE_COLUNAS - (17 + i) / 2, ++j);
                    cprintf("Seus pontos: %d pts", coletor_pontos);
                    j += 2;
                }

                gotoxy(METADE_COLUNAS - 15, ++j);
                cprintf("Precione b para voltar ao menu");

                redesenhar = 0;
            } else if (redesenhar == 2) {
                i = METADE_COLUNAS - RECORDE_MAXIMO_TEXTO_LINHA / 2 + 3;
                j = METADE_LINHAS  + novo_recorde_index + 3;

                gotoxy(i, j);
                cprintf("%s", recorde_nomes[novo_recorde_index]);

                gotoxy(i + recorde_nome_letra, j);
                if (temporizador_flag
                    && recorde_nome_letra < TAMANHO_NOME_RECORDE - 1)
                    cprintf("_");

                redesenhar = 0;
            }
        }

    }
    */

    return 0;
}

