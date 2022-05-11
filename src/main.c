/*
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include <conio2.h>
*/
#include "collector.h"


/*********** Definições */
/*
#define LARGURA_COLETOR               10

#define NUMERO_RECORDES               3
#define TAMANHO_NOME_RECORDE          7
#define RECORDE_MAXIMO_TEXTO_LINHA    43
*/

/*********** Variáveis globais */
/*
int tela = TELA_MENU;
int tela_anterior = -1;

int coletor_pos_x;
int coletor_pos_y;
int coletor_pontos;
int vell_coletor;

int bola_pos_x;
int bola_pos_y;
float bola_velocidade;

int recorde_nome_letra = 0;
int novo_recorde_index = -1;
int recorde_maximos[NUMERO_RECORDES] = {0};
char recorde_nomes[NUMERO_RECORDES][TAMANHO_NOME_RECORDE] = {
    "FELIPE", "IGOR  ", "J0NG4B",
};

int dificuldade = 0;

char linha_caracteres[COLUNAS] = {0};
int sair_do_jogo = 0;
int redesenhar = 1;
*/

/*********** Funções */

/* reinicia_jogo
 *
 * Função para reiniciar as variáveis do jogo tanto em caso de game over quanto
 * em caso de ser a primeira execução do jogo
 */
/*
void reinicia_jogo(void)
{
    coletor_pos_x = METADE_COLUNAS - LARGURA_COLETOR / 2;
    coletor_pos_y = LINHAS * 0.8;
    coletor_pontos = 0;

    bola_pos_x = 1 + rand() % COLUNAS;
    bola_pos_y = 1;
    bola_velocidade = 0.3;

    novo_recorde_index = -1;
    recorde_nome_letra = 0;
}
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

#define MENU_NUMERO_OPCOES   3
#define MENU_NUMERO_LEGENDAS 3
#define MENU_TAMANHO_TEXTOS  30

struct tela_menu {
    int opcao_selecionada;
    struct janela janela_sair;
};

struct tela_menu processa_eventos_tela_menu(struct tela_menu menu, int tecla)
{
    if (menu.janela_sair.visivel) {
        menu.janela_sair = processa_eventos_janela(menu.janela_sair, tecla);

        switch (menu.janela_sair.botao_clicado) {
        case BOTAO_ACEITAR:
            //sair_do_jogo = 1;
            break;

        default:
            break;
        }
    } else if (tecla == '\r') {
        switch (menu.opcao_selecionada) {
        case 0:
            clear_color1 = BLACK;
            clear_color2 = BLACK;

            tela = TELA_DIFICULDADES;
            break;

        case 1:
            clear_color1 = BLACK;
            clear_color2 = BLACK;

            tela = TELA_REGRAS;
            break;

        default:
            menu.janela_sair = mostra_janela(menu.janela_sair);
        }
    } else if (tecla == 'w') {
        menu.opcao_selecionada -= menu.opcao_selecionada > 0;
    } else if (tecla == 's') {
        menu.opcao_selecionada += menu.opcao_selecionada < 2;
    } else {
        redesenhar -= redesenhar > 0;
    }

    return menu;
}

void desenha_tela_menu(struct tela_menu menu)
{
    char legendas[MENU_NUMERO_LEGENDAS][MENU_TAMANHO_TEXTOS] = {
        "enter: confirmar selecao", "s: seta para baixo", "w: seta para cima",
    };

    char opcoes[MENU_NUMERO_OPCOES][MENU_TAMANHO_TEXTOS] = {
        "Iniciar", "Regras", "Sair",
    };

    int i;

    clrscr();
    textcolor(WHITE);
    desenha_nome_jogo(METADE_COLUNAS - 36, LINHAS * 0.3);

    for (i = 0; i < MENU_NUMERO_OPCOES; ++i) {
        if (i == menu.opcao_selecionada)
            textcolor(RED);
        else
            textcolor(WHITE);

        gotoxy(METADE_COLUNAS - strlen(opcoes[i]) / 2, LINHAS * 0.3 + 8 + i);
        cprintf("%s", opcoes[i]);
    }

    textcolor(WHITE);
    for (i = 0; i < MENU_NUMERO_LEGENDAS; ++i) {
        gotoxy(1, LINHAS - i);
        cprintf("%s", legendas[i]);
    }
}

void desenha_tela_regras(void)
{
#define NUMERO_LINHAS_REGRAS 10
#define TAMANHO_TEXTO_REGRA 54

    char regras[NUMERO_LINHAS_REGRAS][TAMANHO_TEXTO_REGRA] = {
        "1. Ao iniciar o jogo as teclas responsaveis por mover",
        "   a barra (coletor) serao a e d.",
        "",
        "2. Quando o jogo for iniciado o objetivo sera nao",
        "   deixar nenhum item cair no chao.",
        "",
        "3. Para contabilizar os pontos sera necessario guiar",
        "   a barra (coletor) ate o item para coleta-lo.",
        "",
        "4. A cada certa pontuacao a dificuldade aumenta.",
    };

    int i;
    int posicao_y_texto = METADE_LINHAS - 4;

    textcolor(RED);

    gotoxy(METADE_COLUNAS - 3, posicao_y_texto - 2);
    cprintf("REGRAS");

    textcolor(WHITE);
    for (i = 0; i < NUMERO_LINHAS_REGRAS; ++i) {
        gotoxy(METADE_COLUNAS - TAMANHO_TEXTO_REGRA / 2, posicao_y_texto++);
        cprintf("%s", regras[i]);
    }

    gotoxy(METADE_COLUNAS - 2, posicao_y_texto + 2);
    cprintf("<< b");

#undef NUMERO_LINHAS_REGRAS
#undef TAMANHO_TEXTO_REGRA
}
*/

int main(void)
{
    struct collector collector;

    collector = novo_collector();
    roda_collector(collector);

    /*
    int clear_color1, clear_color2;


    char temporizador_flag = 0;
    clock_t temporizador = clock();

    srand(time(NULL));
    setlocale(LC_ALL, "C.UTF-8");
    reinicia_jogo();

    for (i = 0; i < COLUNAS; ++i)
        linha_caracteres[i] = ' ';

    desenha_tela_inicial();

    clear_color1 = BLACK;
    clear_color2 = BLACK;

    struct janela menu_janela_sair = nova_janela(20, 10,
        "Deseja realmente sair?",
        "Caso saia do jogo todo seu progresso sera perdido!");

    menu_janela_sair = adiciona_botao_janela(menu_janela_sair, BOTAO_ACEITAR, "Sim");
    menu_janela_sair = adiciona_botao_janela(menu_janela_sair, BOTAO_NEGAR, "Nao");

    while (!sair_do_jogo) {
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

        if (tecla == 'q')
            break;

        if (tela == TELA_MENU) {
            redesenhar += 1;

            if (menu_janela_sair.visivel) {
                menu_janela_sair = desenha_janela(menu_janela_sair);
            } else if (redesenhar) {
                desenha_tela_menu();
                redesenhar = 0;
            }
        } else if (tela == TELA_DIFICULDADES) {
            redesenhar += 1;

            if (tecla == 'a') {
                dificuldade -= dificuldade > 0;
            } else if (tecla == 'd') {
                dificuldade += dificuldade < 2;
            } else if (tecla == 'b') {
                clear_color1 = BLACK;
                clear_color2 = BLACK;

                tela = TELA_MENU;
            } else if (tecla == '\r') {
                clear_color1 = BLUE;
                clear_color2 = GREEN;

                tela = TELA_JOGO;
            } else {
                redesenhar -= redesenhar > 0;
            }

            if (redesenhar) {
                j = METADE_LINHAS - 4;


                textcolor(WHITE);

                gotoxy(METADE_COLUNAS - 7, j++);
                cprintf("Nivel inicial");

                if (dificuldade == 0)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(METADE_COLUNAS - 11, ++j);
                cprintf("Facil");

                if (dificuldade == 1)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(METADE_COLUNAS - 3, j);
                cprintf("Medio");

                if (dificuldade == 2)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(METADE_COLUNAS + 5, j++);
                cprintf("Dificil");

                j += 2;

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

                gotoxy(1, LINHAS - 1);
                cprintf("b: volta para o menu");

                gotoxy(1, LINHAS - 0);
                cprintf("enter: seleciona o nivel inicial");

                redesenhar = 0;
            }
        } else if (tela == TELA_JOGO) {
            redesenhar += 1;

            if (tecla == 'b') {
                tela = TELA_MENU;

                clear_color1 = BLACK;
                clear_color2 = BLACK;
            } else if (tecla == 'd') {
                coletor_pos_x += coletor_pos_x + LARGURA_COLETOR <= COLUNAS;
            } else if (tecla == 'a') {
                 coletor_pos_x -= coletor_pos_x > 1;
            } else {
                redesenhar -= redesenhar > 0;
            }

            if ((clock() - temporizador) / (double) CLOCKS_PER_SEC > bola_velocidade) {
                ++bola_pos_y;
                redesenhar = 1;
                temporizador = clock();
            }

            if (coletor_pontos == 0) {
                bola_velocidade = 0.3 / (dificuldade + 1);
            } else if (coletor_pontos == 5) {
                bola_velocidade = 0.2 / (dificuldade + 1);
            } else if (coletor_pontos == 10) {
                bola_velocidade = 0.1 / (dificuldade + 1);
            } else if (coletor_pontos == 30) {
                bola_velocidade = 0.005 / (dificuldade + 1);
            } else if(coletor_pontos == 40) {
                bola_velocidade = 0.003 / (dificuldade + 1);
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
            if (tecla == 'b') {
                tela = TELA_MENU;

                clear_color1 = BLACK;
                clear_color2 = BLACK;
            }

            if (redesenhar) {
                desenha_tela_regras();
                redesenhar = 0;
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

        if (kbhit())
            tecla = getch();
        else
            tecla = 0;

        gotoxy(METADE_COLUNAS, LINHAS);
    }
    */

    return 0;
}

