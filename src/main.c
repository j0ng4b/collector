#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include <conio2.h>

/*********** Definições */
#define COLUNAS                       80
#define LINHAS                        24

#define METADE_COLUNAS                (COLUNAS / 2)
#define METADE_LINHAS                 (LINHAS / 2)

#define LARGURA_COLETOR               10

#define TELA_MENU                     0
#define TELA_JOGO                     1
#define TELA_REGRAS                   2
#define TELA_GAMEOVER                 3
#define TELA_DIFICULDADES             4

#define NUMERO_RECORDES               3
#define TAMANHO_NOME_RECORDE          7
#define RECORDE_MAXIMO_TEXTO_LINHA    43

#define JANELA_LARGURA_MAXIMA         (COLUNAS * 80 / 100)
#define JANELA_ALTURA_MAXIMA          (LINHAS * 80 / 100)
#define JANELA_NUMERO_BOTOES          3
#define JANELA_TAMANHO_TITULO         (JANELA_LARGURA_MAXIMA - 10)
#define JANELA_TAMANHO_MENSAGEM_LINHA (JANELA_LARGURA_MAXIMA - 4)
#define JANELA_TAMANHO_MENSAGEM       \
    ((JANELA_LARGURA_MAXIMA - 4) * (JANELA_ALTURA_MAXIMA - 4))

#define BOTAO_TAMANHO_TEXTO           10

/*********** Variáveis globais */
int menu_opcao_atual = 0;

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


/*********** Funções */

/* msleep
 *
 * Função para pausar a execução do programa em alguns milisegundos.
 */
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

/* reinicia_jogo
 *
 * Função para reiniciar as variáveis do jogo tanto em caso de game over quanto
 * em caso de ser a primeira execução do jogo
 */
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

/* animacao_gameover
 *
 * Função para criar uma animação de várias bolinhas caindo.
 */
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

/* novo_recorde
 *
 * Função para verificar se o jogador conseguiu atingir uma nova pontuação
 * máxima. Se ele conseguiu retorna a posição dessa nova pontuação na lista de
 * recordes e atribuí essa nova pontuação na lista de recordes, do contrário
 * apenas retorna -1.
 */
int novo_recorde(void)
{
    int i, j, k;

    for (i = 0; i < NUMERO_RECORDES; ++i) {
        if (recorde_maximos[i] < coletor_pontos) {
            /* Move todos os recordes uma posição para baixo. Na posição do novo
             * recorde a nova pontuação é atribuída e o antigo nome é
             * substituído.
             */
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

/* TODO: funções para cada tela */

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

enum tipo_botao {
    BOTAO_NEGAR,
    BOTAO_ACEITAR,
    BOTAO_CANCELAR,
    BOTAO_NULO,
};

struct botao {
    char texto[BOTAO_TAMANHO_TEXTO];
    int tamanho_texto;

    int x, y;
    enum tipo_botao tipo;
};

struct janela {
    int x, y;

    int largura;
    int altura;

    char titulo[JANELA_TAMANHO_TITULO];
    char mensagem[JANELA_ALTURA_MAXIMA][JANELA_TAMANHO_MENSAGEM_LINHA];
    int mensagem_linhas;

    int redesenhar;
    int visivel;
    int criada;

    enum tipo_botao botao_clicado;
    struct botao botoes[JANELA_NUMERO_BOTOES];
    int numero_botoes;
    int tamanho_texto_botoes;
    int botao_selecionado;
};

struct janela nova_janela(int largura, int altura, char titulo[], char mensagem[])
{
    int i, msg_pos;
    struct janela janela = { 0 };

    if (strlen(titulo) > JANELA_TAMANHO_TITULO)
        return janela;

    /* Ajusta a largura da janela para uma largura válida, isto é, dentro de um
     * limite máximo e mínimo.
     */
    if (largura == 0 || largura > JANELA_LARGURA_MAXIMA)
        largura = JANELA_LARGURA_MAXIMA;
    else if (largura < (int) strlen(titulo))
        largura = strlen(titulo) + 10;

    /* Faz o mesmo que foi feito com a largura, mas agora com a altura */
    if (altura == 0 || altura > JANELA_ALTURA_MAXIMA)
        altura = JANELA_ALTURA_MAXIMA;

    janela.largura = largura;
    janela.altura = altura;

    /* Copiá a mensagem para a área de mensagem da janela respeitando o tamanho
     * máximo da janela, caso esse tamanho seja ultrapassado a mensagem
     * automaticamente recebe uma quebra de linha.
     */
    for (janela.mensagem_linhas = i = msg_pos = 0; mensagem[i] != '\0'; ++i) {
        janela.mensagem[janela.mensagem_linhas][msg_pos] = mensagem[i];

        if (msg_pos + 1 >= janela.largura - 4) {
            janela.mensagem_linhas++;
            msg_pos = 0;
        } else {
            msg_pos++;
        }
    }

    strncpy(janela.titulo, titulo, JANELA_TAMANHO_TITULO);

    janela.mensagem_linhas++;

    janela.x = METADE_COLUNAS - janela.largura / 2;
    janela.y = METADE_LINHAS - janela.altura / 2;

    janela.redesenhar = 1;
    janela.criada = 1;

    return janela;
}

struct janela adiciona_botao_janela(struct janela janela, enum tipo_botao tipo,
    char texto_botao[BOTAO_TAMANHO_TEXTO])
{
    int i = 0;
    int tamanho_texto_botoes;
    struct botao botao = { 0 };

    botao.tamanho_texto = strlen(texto_botao);
    if (botao.tamanho_texto >= BOTAO_TAMANHO_TEXTO)
        return (struct janela) { 0 };

    strncpy(botao.texto, texto_botao, BOTAO_TAMANHO_TEXTO);
    botao.tipo = tipo;
    botao.y = janela.altura - 2;

    janela.botoes[janela.numero_botoes] = botao;
    janela.tamanho_texto_botoes += botao.tamanho_texto + (janela.numero_botoes > 0);
    janela.numero_botoes++;

    tamanho_texto_botoes = janela.tamanho_texto_botoes;
    janela.botoes[0].x = janela.largura / 2 - tamanho_texto_botoes / 2;
    for (i = 1; i < janela.numero_botoes; ++i)
        janela.botoes[i].x = janela.botoes[i - 1].x + janela.botoes[i - 1].tamanho_texto + 1;

    return janela;
}

struct janela desenha_janela(struct janela janela)
{
    int x, y;

    if (!janela.redesenhar)
        return janela;

    janela.redesenhar = 0;

    desenha_moldura(0, janela.x, janela.y, janela.largura, janela.altura);

    for (y = 0; y < janela.altura; ++y) {
        if (y > 1 && y - 2 < janela.mensagem_linhas) {
            gotoxy(janela.x + 2, janela.y + y);
            cprintf("%-*s", janela.largura - 3, janela.mensagem[y - 2]);
        }
    }

    for (x = 0; x < janela.numero_botoes; ++x) {
        if (x == janela.botao_selecionado)
            textbackground(RED);
        else
            textbackground(BLACK);

        gotoxy(janela.x + janela.botoes[x].x, janela.y + janela.botoes[x].y);
        cprintf("%s", janela.botoes[x].texto);
    }

    textbackground(BLACK);
    gotoxy(janela.x + 2, janela.y);
    cprintf(" %s ", janela.titulo);

    return janela;
}

struct janela processa_eventos_janela(struct janela janela, int tecla)
{
    if (!janela.visivel)
        return janela;

    janela.redesenhar = 1;
    janela.botao_clicado = BOTAO_NULO;

    if (tecla == 'a' && janela.botao_selecionado > 0) {
        janela.botao_selecionado--;
    } else if (tecla == 'd' && janela.botao_selecionado + 1 < janela.numero_botoes) {
        janela.botao_selecionado++;
    } else if (tecla == '\r') {
        janela.visivel = 0;
        janela.botao_clicado = janela.botoes[janela.botao_selecionado].tipo;
        janela.botao_selecionado = 0;
    } else {
        janela.redesenhar = 0;
    }

    return janela;
}

struct janela mostra_janela(struct janela janela)
{
    janela.visivel = 1;
    janela.redesenhar = 1;

    return janela;
}

int main(void)
{
    int clear_color1, clear_color2;

    int i, j, k;
    char tecla = 0;

    char temporizador_flag = 0;
    clock_t temporizador = clock();

    srand(time(NULL));
    setlocale(LC_ALL, "C.UTF-8");
    reinicia_jogo();

    for (i = 0; i < COLUNAS; ++i)
        linha_caracteres[i] = ' ';

    for (i = 1; i <= LINHAS; ++i) {
        if (i == 1 || i == LINHAS) {
            gotoxy(1, i);
            cprintf("+");

            gotoxy(COLUNAS, i);
            cprintf("+");
        } else {
            gotoxy(1, i);
            cprintf(":");

            gotoxy(COLUNAS, i);
            cprintf(":");
        }
    }

    i = METADE_COLUNAS - 36;
    j = METADE_LINHAS  - 3;

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
        gotoxy(METADE_COLUNAS - 18, LINHAS * 0.8);

        if ((i = !i))
            cprintf("Precione qualquer tecla para iniciar!");
        else
            cprintf("                                     ");

        gotoxy(METADE_COLUNAS, LINHAS);
        msleep(500);
    } while (!kbhit());

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

        if (tela == TELA_MENU) {
            redesenhar += 1;

            if (menu_janela_sair.visivel) {
                menu_janela_sair = processa_eventos_janela(menu_janela_sair, tecla);

                switch (menu_janela_sair.botao_clicado) {
                case BOTAO_ACEITAR:
                    sair_do_jogo = 1;
                    break;

                default:
                    break;
                }
            } else if (tecla == '\r') {
                switch (menu_opcao_atual) {
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
                    menu_janela_sair = mostra_janela(menu_janela_sair);
                }
            } else if (tecla == 'w') {
                menu_opcao_atual -= menu_opcao_atual > 0;
            } else if (tecla == 's') {
                menu_opcao_atual += menu_opcao_atual < 2;
            } else {
                redesenhar -= redesenhar > 0;
            }

            if (menu_janela_sair.visivel) {
                menu_janela_sair = desenha_janela(menu_janela_sair);
            } else if (redesenhar) {
                clrscr();

                textcolor(WHITE);

                i = METADE_COLUNAS - 36;
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

                gotoxy(METADE_COLUNAS - 3, ++j);
                cprintf("Iniciar");

                if (menu_opcao_atual == 1)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(METADE_COLUNAS - 3, ++j);
                cprintf("Regras");

                if (menu_opcao_atual == 2)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(METADE_COLUNAS - 2, ++j);
                cprintf("Sair");

                textcolor(WHITE);
                gotoxy(1, LINHAS - 2);
                cprintf("w: seta para cima");

                gotoxy(1, LINHAS - 1);
                cprintf("s: seta para baixo");

                gotoxy(1, LINHAS - 0);
                cprintf("enter: confirmar selecao");

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

                /* TODO: uma tela mais bonitinha com uma descrição do nível */

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
                i = METADE_COLUNAS - 26;
                j = METADE_LINHAS  - 4;

                textcolor(RED);

                gotoxy(METADE_COLUNAS - 3, j - 2);
                cprintf("REGRAS");

                textcolor(WHITE);

                gotoxy(i, j++);
                cprintf("1. Ao iniciar o jogo as teclas responsaveis por mover");
                gotoxy(i, j++);
                cprintf("   a barra (coletor) serao a e d.");

                ++j;
                gotoxy(i, j++);
                cprintf("2. Quando o jogo for iniciado o objetivo sera nao");
                gotoxy(i, j++);
                cprintf("   deixar nenhum item cair no chao.");

                ++j;
                gotoxy(i, j++);
                cprintf("3. Para contabilizar os pontos sera necessario guiar");
                gotoxy(i, j++);
                cprintf("   a barra (coletor) ate o item para coleta-lo.");

                ++j;
                gotoxy(i, j++);
                cprintf("4. A cada certa pontuacao a dificuldade aumenta.");
                ++j;

                gotoxy(METADE_COLUNAS - 2, j + 2);
                cprintf("<< b");

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

    return 0;
}

