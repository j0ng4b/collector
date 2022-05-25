#include <string.h>
#include <time.h>
#include <conio2.h>
#include "janela.h"

int janela_nova(struct janela *janela, struct janela_config *config)
{
    int i;

    int inicio_copia = 0;
    int ultimo_espaco_encontrado = 0;

    int tamanho_mensagem_em_linha = 0;
    int tamanho_mensagem = strlen(config->texto.mensagem);

    memset(janela, 0, sizeof(struct janela));


    if (config->tipo == JANELA_COMUM) {
        if (strlen(config->texto.titulo) > JANELA_TAMANHO_TITULO)
            return -1;

        /* Ajusta a largura da janela para uma largura válida, isto é, dentro de um
         * limite máximo e mínimo.
         */
        if (config->tamanho.largura == 0
            || config->tamanho.largura > JANELA_LARGURA_MAXIMA)
            config->tamanho.largura = JANELA_LARGURA_MAXIMA;
        else if (config->tamanho.largura < (int) strlen(config->texto.titulo))
            config->tamanho.largura = strlen(config->texto.titulo) + 10;

        /* Faz o mesmo que foi feito com a largura, mas agora com a altura */
        if (config->tamanho.altura == 0 || config->tamanho.altura > JANELA_ALTURA_MAXIMA)
            config->tamanho.altura = JANELA_ALTURA_MAXIMA;

        janela->tamanho.largura = config->tamanho.largura;
        janela->tamanho.altura = config->tamanho.altura;

        strncpy(janela->texto.titulo, config->texto.titulo, JANELA_TAMANHO_TITULO);
        /* Copiá a mensagem para a área de mensagem da janela respeitando o tamanho
         * máximo da janela, caso esse tamanho seja ultrapassado a mensagem
         * automaticamente recebe uma quebra de linha.
         */
        for (i = 0; i < tamanho_mensagem; ++i) {
            if (config->texto.cru) {
                tamanho_mensagem_em_linha++;
                if (tamanho_mensagem_em_linha > janela->tamanho.largura - 4
                    || i + 1 >= tamanho_mensagem) {
                    strncpy(janela->texto.mensagem[janela->texto.mensagem_linhas],
                        config->texto.mensagem + inicio_copia,
                        janela->tamanho.largura - 4);

                    janela->texto.mensagem_linhas++;

                    tamanho_mensagem_em_linha = 0;
                    inicio_copia += janela->tamanho.largura - 4;
                }
            } else {
                if (config->texto.mensagem[i] == ' ')
                    ultimo_espaco_encontrado = i;

                tamanho_mensagem_em_linha++;
                if (tamanho_mensagem_em_linha > janela->tamanho.largura - 4
                    || i + 1 >= tamanho_mensagem) {
                    if (i + 1 >= tamanho_mensagem)
                        ultimo_espaco_encontrado = tamanho_mensagem;

                    strncpy(janela->texto.mensagem[janela->texto.mensagem_linhas],
                        config->texto.mensagem + inicio_copia,
                        ultimo_espaco_encontrado - inicio_copia);

                    janela->texto.mensagem_linhas++;

                    tamanho_mensagem_em_linha = 0;
                    inicio_copia = ultimo_espaco_encontrado + 1;
                }
            }
        }

        janela->texto.mensagem_linhas++;
    } else if (config->tipo == JANELA_TEMPORIZADORA) {
        janela->tempo.maximo = config->tempo;
        janela->tempo.atual = config->tempo;

        for (; config->tempo; ++janela->tamanho.largura, config->tempo /= 10);
        janela->tamanho.largura += 4;
        janela->tamanho.altura = 5;
    }

    janela->tipo = config->tipo;

    janela->posicao.x = METADE_COLUNAS - janela->tamanho.largura / 2;
    janela->posicao.y = METADE_LINHAS - janela->tamanho.altura / 2;

    janela->cores = config->cores;
    janela->redesenhar = 1;
    janela->visivel = 0;

    return 0;
}

void janela_adiciona_botao(struct janela *janela,
    enum janela_tipo_botao tipo, char texto_botao[JANELA_TAMANHO_TEXTO_BOTAO])
{
    int i = 0;
    struct janela_botao botao = { 0 };

    botao.texto.tamanho = strlen(texto_botao);
    if (botao.texto.tamanho >= JANELA_TAMANHO_TEXTO_BOTAO)
        return;

    strncpy(botao.texto.texto, texto_botao, JANELA_TAMANHO_TEXTO_BOTAO);
    botao.tipo = tipo;
    botao.posicao.y = janela->tamanho.altura - 2;

    janela->botao.botoes[janela->botao.quantidade++] = botao;

    janela->botao.alinhamento += botao.texto.tamanho;
    janela->botao.alinhamento += janela->botao.quantidade > 1 ? 3 : 2;

    janela->botao.botoes[0].posicao.x =
        janela->tamanho.largura / 2 - janela->botao.alinhamento / 2;
    for (i = 1; i < janela->botao.quantidade; ++i)
        janela->botao.botoes[i].posicao.x =
            janela->botao.botoes[i - 1].posicao.x +
            janela->botao.botoes[i - 1].texto.tamanho + 2;
}

int janela_atualiza(struct janela *janela, int tecla)
{
    int tmp;
    if (!janela->visivel)
        return JANELA_BOTAO_NULO;

    if (janela->tipo == JANELA_COMUM) {
        if (tecla == 'a' && janela->botao.indice_botao_atual > 0) {
            janela->botao.indice_botao_atual--;
            janela->redesenhar = 1;
        } else if (tecla == 'd'
            && janela->botao.indice_botao_atual + 1 < janela->botao.quantidade) {
            janela->botao.indice_botao_atual++;
            janela->redesenhar = 1;
        } else if (tecla == '\r') {
            tmp = janela->botao.indice_botao_atual;

            janela->visivel = 0;
            janela->botao.indice_botao_atual = 0;

            return janela->botao.botoes[tmp].tipo;
        } else {
            return JANELA_BOTAO_NULO;
        }
    } else if (janela->tipo == JANELA_TEMPORIZADORA) {
        if ((clock() - janela->tempo.temporizador) / (double) CLOCKS_PER_SEC > 1) {
            janela->redesenhar = 1;
            janela->tempo.atual -= janela->tempo.temporizador != 0 ? 1 : 0;
            janela->tempo.temporizador = clock();
        }

        if (janela->tempo.atual <= 0) {
            janela->tempo.atual = 0;
            janela->visivel = 0;
        }
    }

    return JANELA_BOTAO_NULO;
}

void janela_desenha(struct janela *janela)
{
    int x, y;

    if (!janela->redesenhar)
        return;

    janela->redesenhar = 0;

    textcolor(janela->cores.texto);
    textbackground(janela->cores.fundo);
    desenha_moldura(0, janela->posicao.x, janela->posicao.y,
        janela->tamanho.largura, janela->tamanho.altura);

    if (janela->tipo == JANELA_COMUM) {
        for (y = 0; y < janela->tamanho.altura; ++y) {
            if (y > 1 && y - 2 < janela->texto.mensagem_linhas) {
                gotoxy(janela->posicao.x + 2, janela->posicao.y + y);
                cprintf("%-*s", janela->tamanho.largura - 3,
                    janela->texto.mensagem[y - 2]);
            }
        }

        for (x = 0; x < janela->botao.quantidade; ++x) {
            if (x == janela->botao.indice_botao_atual)
                textbackground(RED);
            else
                textbackground(janela->cores.fundo);

            gotoxy(janela->posicao.x + janela->botao.botoes[x].posicao.x,
                janela->posicao.y + janela->botao.botoes[x].posicao.y);
            cprintf(" %s ", janela->botao.botoes[x].texto.texto);
        }

        textbackground(janela->cores.fundo);
        gotoxy(janela->posicao.x + 2, janela->posicao.y);
        cprintf(" %s ", janela->texto.titulo);
    } else if (janela->tipo) {
        gotoxy(janela->posicao.x + janela->tamanho.largura / 2 -
            (janela->tamanho.largura - 4) / 2, janela->posicao.y + 2);
        cprintf("%0*d", janela->tamanho.largura - 4, janela->tempo.atual);
    }
}

void janela_reiniciar_tempo(struct janela *janela)
{
    janela->tempo.atual = janela->tempo.maximo;
    janela->tempo.temporizador = 0;

    janela->visivel = 1;
    janela->redesenhar = 1;
}

void janela_mostrar(struct janela *janela)
{
    janela->visivel = 1;
    janela->redesenhar = 1;
}

