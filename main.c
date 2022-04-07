#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include <conio2.h>

#ifdef _WIN32
#   include <windows.h>
#elif defined(__linux__)
#   include <errno.h>

int Sleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
#endif


/*********** Definições */
#define COLUNAS                    80
#define LINHAS                     24

#define LARGURA_COLETOR            10

#define TELA_MENU                  0
#define TELA_JOGO                  1
#define TELA_REGRAS                2
#define TELA_GAMEOVER              3
#define TELA_RECORDE               4

#define NUMERO_RECORDES            6
#define TAMANHO_NOME_RECORDE       4
#define RECORDE_MAXIMO_TEXTO_LINHA 40


/*********** Variáveis globais */
int menu_opcao_atual = 0;

int tela = TELA_MENU;
int tela_anterior = -1;

int coletor_pos_x;
int coletor_pos_y;
int coletor_pontos;

int bola_pos_x;
int bola_pos_y;

char recorde_nomes[NUMERO_RECORDES][TAMANHO_NOME_RECORDE] = {
    "JON", "JON", "JON", "JON", "JON", "JON",
};

int recorde_maximas[NUMERO_RECORDES] = {0};

int sair_do_jogo = 0;
int redesenhar = 1;


/*********** Funções */

/* reinicia_jogo
 *
 * Função para reiniciar as variáveis do jogo tanto em caso de gameover quanto
 * em caso de ser a primeira execução do jogo
 */
void reinicia_jogo(void)
{
    coletor_pos_x = COLUNAS / 2 - LARGURA_COLETOR / 2;
    coletor_pos_y = LINHAS * 0.8;
    coletor_pontos = 0;

    bola_pos_x = 1 + rand() % COLUNAS;
    bola_pos_y = 2;
}

void animacao_gameover(void)
{
    int bolas[COLUNAS + 1] = {0};

    while (bolas[COLUNAS] <= LINHAS) {
        /* Declarações no for não é ANSI C, ou seja, é algo inválido */
        for (int i = 1; i <= COLUNAS; ++i) {
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
                bolas[i] += bolas[i] <= LINHAS && bolas[i - 1] > 2;
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

int main()
{
    int clear_color1, clear_color2;

    int i, j;
    char key = 0;

    clock_t temporizador = clock();

    srand(time(NULL));
    setlocale(LC_ALL, "pt_BR.UTF-8");

    reinicia_jogo();
    for (i = 1; i <= LINHAS; ++i) {
        for (j = 1; j <= COLUNAS; ++j) {
            gotoxy(j, i);

            if (i == 1 || i == LINHAS) {
                if (j == 1 || j == COLUNAS)
                    cprintf("+");
                else
                    cprintf(" ");
            } else {
                if (j == 1 || j == COLUNAS)
                    cprintf(":");
                else
                    cprintf(" ");
            }
        }
    }

    i = COLUNAS / 2 - 36;
    j = LINHAS / 2 - 3;

    gotoxy(i, j++);
    cprintf("_______ _______ ___     ___     _______ _______ _______ _______ _______");
    gotoxy(i, j++);
    cprintf("|   _   |   _   |   |   |   |   |   _   |   _   |       |   _   |   _   \\");
    gotoxy(i, j++);
    cprintf("|.  1___|.  |   |.  |   |.  |   |.  1___|.  1___|.|   | |.  |   |.  l   /");
    gotoxy(i, j++);
    cprintf("|.  |___|.  |   |.  |___|.  |___|.  __)_|.  |___`-|.  |-|.  |   |.  _   1");
    gotoxy(i, j++);
    cprintf("|:  1   |:  1   |:  1   |:  1   |:  1   |:  1   | |:  | |:  1   |:  |   |");
    gotoxy(i, j++);
    cprintf("|::.. . |::.. . |::.. . |::.. . |::.. . |::.. . | |::.| |::.. . |::.|:. |");
    gotoxy(i, j++);
    cprintf("`-------`-------`-------`-------`-------`-------' `---' `-------`--- ---'");

    i = 1;
    do {
        gotoxy(COLUNAS / 2 - 18, LINHAS * 0.8);

        if ((i = !i))
            cprintf("Precione qualquer tecla para iniciar!");
        else
            cprintf("                                     ");

        gotoxy(COLUNAS / 2, LINHAS);
        Sleep(500);
    } while (!kbhit());

    clear_color1 = BLACK;
    clear_color2 = BLACK;

    while (!sair_do_jogo) {
        if (tela_anterior != tela) {
            tela_anterior = tela;
            redesenhar = 1;

            if (tela == TELA_GAMEOVER) {
                animacao_gameover();
            } else {
                for (i = 1; i <= LINHAS / 2; ++i) {
                    for (j = 1; j <= COLUNAS; ++j) {
                        textbackground(WHITE);
                        gotoxy(j, i);
                        cprintf(" ");

                        gotoxy(j, LINHAS - (i - 1));
                        cprintf(" ");

                        if (i > 1) {
                            textbackground(clear_color1);
                            textcolor(clear_color1);
                            gotoxy(j, i - 1);
                            cprintf(" ");

                            if (i <= LINHAS * 0.2)
                                textbackground(clear_color2);

                            gotoxy(j, LINHAS - (i - 2));
                            cprintf(" ");
                        }
                    }

                    gotoxy(COLUNAS / 2, LINHAS / 2);
                    Sleep(20);
                }

                textbackground(clear_color1);
                for (j = COLUNAS / 2; j > 0; --j) {
                    for (i = 0; i < 2; ++i) {
                        gotoxy(j, LINHAS / 2 + i);
                        cprintf(" ");

                        gotoxy(COLUNAS - (j - 1), LINHAS / 2 + i);
                        cprintf(" ");
                    }

                    gotoxy(COLUNAS / 2, LINHAS);
                    Sleep(15);
                }
            }

            while (kbhit()) getch();
        }

        if (tela == TELA_MENU) {
            redesenhar += 1;

            if (key == '\r') {
                switch (menu_opcao_atual) {
                case 0:
                    clear_color1 = BLUE;
                    clear_color2 = GREEN;

                    tela = TELA_JOGO;
                    break;

                case 1:
                    clear_color1 = BLACK;
                    clear_color2 = BLACK;

                    tela = TELA_REGRAS;
                    break;

                case 2:
                    clear_color1 = BLACK;
                    clear_color2 = BLACK;

                    tela = TELA_RECORDE;
                    break;

                default:
                    sair_do_jogo = 1;
                }
            }

            if (key == 'w')
                menu_opcao_atual -= menu_opcao_atual > 0;
            else if (key == 's')
                menu_opcao_atual += menu_opcao_atual < 3;
            else
                redesenhar -= redesenhar > 0;

            if (redesenhar) {
                textcolor(WHITE);

                i = COLUNAS / 2 - 36;
                j = LINHAS * 0.3;

                gotoxy(i, j++);
                cprintf("_______ _______ ___     ___     _______ _______ _______ _______ _______");
                gotoxy(i, j++);
                cprintf("|   _   |   _   |   |   |   |   |   _   |   _   |       |   _   |   _   \\");
                gotoxy(i, j++);
                cprintf("|.  1___|.  |   |.  |   |.  |   |.  1___|.  1___|.|   | |.  |   |.  l   /");
                gotoxy(i, j++);
                cprintf("|.  |___|.  |   |.  |___|.  |___|.  __)_|.  |___`-|.  |-|.  |   |.  _   1");
                gotoxy(i, j++);
                cprintf("|:  1   |:  1   |:  1   |:  1   |:  1   |:  1   | |:  | |:  1   |:  |   |");
                gotoxy(i, j++);
                cprintf("|::.. . |::.. . |::.. . |::.. . |::.. . |::.. . | |::.| |::.. . |::.|:. |");
                gotoxy(i, j++);
                cprintf("`-------`-------`-------`-------`-------`-------' `---' `-------`--- ---'");

                if (menu_opcao_atual == 0)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(COLUNAS / 2 - 3, ++j);
                cprintf("Iniciar");

                if (menu_opcao_atual == 1)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(COLUNAS / 2 - 3, ++j);
                cprintf("Regras");

                if (menu_opcao_atual == 2)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(COLUNAS / 2 - 4, ++j);
                cprintf("Recordes");

                if (menu_opcao_atual == 3)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(COLUNAS / 2 - 2, ++j);
                cprintf("Sair");

                textcolor(WHITE);
                gotoxy(1, LINHAS - 2);
                cprintf("w: seta para cima");

                gotoxy(1, LINHAS - 1);
                cprintf("s: seta para baixo");

                gotoxy(1, LINHAS - 0);
                cprintf("↲: confirmar seleção");

                redesenhar = 0;
            }
        } else if (tela == TELA_JOGO) {
            redesenhar += 1;

            if (key == 'b') {
                tela = TELA_MENU;

                clear_color1 = BLACK;
                clear_color2 = BLACK;
            } else if (key == 'd') {
                coletor_pos_x += coletor_pos_x + LARGURA_COLETOR <= COLUNAS;
            } else if (key == 'a') {
                coletor_pos_x -= coletor_pos_x > 1;
            } else {
                redesenhar -= redesenhar > 0;
            }

            if ((clock() - temporizador) / (double) CLOCKS_PER_SEC > 0.3) {
                ++bola_pos_y;
                redesenhar = 1;
                temporizador = clock();
            }

            if (bola_pos_y == coletor_pos_y) {
                if (bola_pos_x >= coletor_pos_x
                        && bola_pos_x < coletor_pos_x + LARGURA_COLETOR) {
                    textbackground(BLUE);
                    gotoxy(bola_pos_x, bola_pos_y - 1);
                    cprintf(" ");

                    bola_pos_x = 1 + rand() % COLUNAS;
                    bola_pos_y = 4;
                    ++coletor_pontos;
                } else {
                    tela = TELA_GAMEOVER;

                    clear_color1 = BLACK;
                    clear_color2 = BLACK;
                }
            }

            if (redesenhar) {
                textcolor(BLACK);
                textbackground(BLUE);
                gotoxy(1, 1);
                cprintf("Pontos: %d", coletor_pontos);

                textbackground(YELLOW);
                gotoxy(bola_pos_x, bola_pos_y);
                cprintf(" ");

                textbackground(BLUE);
                gotoxy(bola_pos_x, bola_pos_y - 1);
                cprintf(" ");

                textbackground(BLUE);
                if (coletor_pos_x > 1) {
                    gotoxy(coletor_pos_x - 1, coletor_pos_y);
                    cprintf(" ");
                }

                if (coletor_pos_x + LARGURA_COLETOR <= COLUNAS) {
                    gotoxy(coletor_pos_x + LARGURA_COLETOR, coletor_pos_y);
                    cprintf(" ");
                }

                textbackground(WHITE);
                for (i = 0; i < LARGURA_COLETOR; ++i) {
                    gotoxy(coletor_pos_x + i, coletor_pos_y);
                    cprintf(" ");
                }

                redesenhar = 0;
            }
        } else if (tela == TELA_REGRAS) {
            if (key == 'b') {
                tela = TELA_MENU;

                clear_color1 = BLACK;
                clear_color2 = BLACK;
            }

            if (redesenhar) {
                i = COLUNAS / 2 - 26;
                j = LINHAS / 2 - 4;

                textcolor(RED);

                gotoxy(COLUNAS / 2 - 3, j - 2);
                cprintf("REGRAS");

                textcolor(WHITE);

                gotoxy(i, j++);
                cprintf("1. Ao iniciar o jogo as teclas responsáveis por mover");
                gotoxy(i, j++);
                cprintf("   a barra (coletor) serão a e d.");

                ++j;
                gotoxy(i, j++);
                cprintf("2. Quando o jogo for iniciado o objetivo será não");
                gotoxy(i, j++);
                cprintf("   deixar nenhum ítem cair no chão.");

                ++j;
                gotoxy(i, j++);
                cprintf("3. Para contabilizar os pontos será necessário guiar");
                gotoxy(i, j++);
                cprintf("   a barra (coletor) até o ítem para coleta-lo.");

                gotoxy(COLUNAS / 2 - 2, j + 2);
                cprintf("<< b");

                redesenhar = 0;
            }
        } else if (tela == TELA_GAMEOVER) {
            if (key == 'b') {
                tela = TELA_MENU;

                clear_color1 = BLACK;
                clear_color2 = BLACK;

                reinicia_jogo();
            }

            if (redesenhar) {
                i = COLUNAS / 2 - 32;
                j = LINHAS / 2 - 9;

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

                gotoxy(COLUNAS / 2 - 8, ++j);
                cprintf("Seus pontos: %4d", coletor_pontos);

                i = COLUNAS / 2 - RECORDE_MAXIMO_TEXTO_LINHA / 2;
                j += 3;

                gotoxy(COLUNAS / 2 - 4, j++);
                cprintf("Recordes");

                /* Declarações no for não é ANSI C, ou seja, é algo inválido */
                for (int k = 0; k < 3; ++k, ++j) {
                    gotoxy(i, j);

                    for (int l = 0; l < RECORDE_MAXIMO_TEXTO_LINHA; ++l)
                        cprintf(".");

                    gotoxy(i, j);
                    for (l = 0; l < TAMANHO_NOME_RECORDE - 1; ++l)
                        cprintf("%c ", toupper(recorde_nomes[k][l]));

                    gotoxy(i + RECORDE_MAXIMO_TEXTO_LINHA - 9, j++);
                    cprintf(" %4d pts", recorde_maximas[k]);
                }

                gotoxy(COLUNAS / 2 - 15, ++j);
                cprintf("Precione b para voltar ao menu");

                redesenhar = 0;
            }
        } else if (tela == TELA_RECORDE) {
            if (key == 'b') {
                tela = TELA_MENU;

                clear_color1 = BLACK;
                clear_color2 = BLACK;
            }

            if (redesenhar) {
                i = COLUNAS / 2 - PONTUACAO_MAXIMO_TEXTO_LINHA / 2;
                j = LINHAS / 2 - (NUMERO_PONTUACOES * 2 + 3) / 2;

                textcolor(RED);

                gotoxy(COLUNAS / 2 - 5, j);
                j += 2;
                cprintf("PONTUAÇÕES");

                textcolor(WHITE);

                /* Declarações no for não é ANSI C, ou seja, é algo inválido */
                for (int k = 0; k < NUMERO_PONTUACOES; ++k, ++j) {
                    gotoxy(i, j);

                    for (int l = 0; l < PONTUACAO_MAXIMO_TEXTO_LINHA; ++l)
                        cprintf(".");

                    gotoxy(i, j);
                    for (l = 0; l < TAMANHO_NOME_PONTUACAO - 1; ++l)
                        cprintf("%c ", toupper(pontuacao_nomes[k][l]));

                    gotoxy(i + PONTUACAO_MAXIMO_TEXTO_LINHA - 9, j++);
                    cprintf(" %4d pts", pontuacao_maximas[k]);
                }

                gotoxy(COLUNAS / 2 - 2, j + 1);
                cprintf("<< b");

                redesenhar = 0;
            }
        }

        if (kbhit())
            key = getch();
        else
            key = 0;

        gotoxy(COLUNAS / 2, LINHAS);
    }

    return 0;
}

