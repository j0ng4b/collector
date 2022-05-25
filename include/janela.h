#ifndef COLLECTOR_JANELA_H
#define COLLECTOR_JANELA_H

#include <time.h>
#include "utilitarios.h"

#define JANELA_NUMERO_BOTOES          3

#define JANELA_LARGURA_MAXIMA         (COLUNAS * 80 / 100)
#define JANELA_ALTURA_MAXIMA          (LINHAS * 80 / 100)

#define JANELA_TAMANHO_TITULO         (JANELA_LARGURA_MAXIMA - 10)
#define JANELA_TAMANHO_MENSAGEM_LINHA (JANELA_LARGURA_MAXIMA - 4)
#define JANELA_TAMANHO_MENSAGEM       \
    ((JANELA_LARGURA_MAXIMA - 4) * (JANELA_ALTURA_MAXIMA - 4))

#define JANELA_TAMANHO_TEXTO_BOTAO    10

enum janela_tipo_botao {
    JANELA_BOTAO_NEGAR,
    JANELA_BOTAO_ACEITAR,
    JANELA_BOTAO_CANCELAR,
    JANELA_BOTAO_NULO
};

enum janela_tipo {
    JANELA_COMUM,
    JANELA_TEMPORIZADORA
};

struct janela_cores {
    int fundo;
    int texto;
};

struct janela_botao {
    struct {
        int x;
        int y;
    } posicao;

    struct {
        char texto[JANELA_TAMANHO_TEXTO_BOTAO];
        int tamanho;
    } texto;

    enum janela_tipo_botao tipo;
};

struct janela_config {
    struct {
        int largura;
        int altura;
    } tamanho;

    struct {
        char titulo[JANELA_TAMANHO_TITULO];
        char mensagem[JANELA_TAMANHO_MENSAGEM];

        int cru;
    } texto;

    int tempo;

    struct janela_cores cores;
    enum janela_tipo tipo;
};

struct janela {
    struct {
        int x;
        int y;
    } posicao;

    struct {
        int largura;
        int altura;
    } tamanho;

    struct {
        char titulo[JANELA_TAMANHO_TITULO];
        char mensagem[JANELA_ALTURA_MAXIMA][JANELA_TAMANHO_MENSAGEM_LINHA];

        int mensagem_linhas;
    } texto;

    struct {
        struct janela_botao botoes[JANELA_NUMERO_BOTOES];

        int indice_botao_atual;
        int alinhamento;
        int quantidade;
    } botao;

    struct {
        int atual;
        int maximo;

        clock_t temporizador;
    } tempo;

    int redesenhar;
    int visivel;

    enum janela_tipo tipo;
    struct janela_cores cores;

    int tamanho_texto_botoes;
};

int janela_nova(struct janela *janela, struct janela_config *config);

void janela_adiciona_botao(struct janela *janela,
    enum janela_tipo_botao tipo, char texto_botao[JANELA_TAMANHO_TEXTO_BOTAO]);

int janela_atualiza(struct janela *janela, int tecla);

void janela_desenha(struct janela *janela);
void janela_reiniciar_tempo(struct janela *janela);
void janela_mostrar(struct janela *janela);

#endif /* COLLECTOR_JANELA_H */

