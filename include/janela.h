#ifndef COLLECTOR_JANELA_H
#define COLLECTOR_JANELA_H

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
    JANELA_BOTAO_NULO,
};

struct janela_botao {
    char texto[JANELA_TAMANHO_TEXTO_BOTAO];
    int tamanho_texto;

    int x, y;
    enum janela_tipo_botao tipo;
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

    enum janela_tipo_botao botao_clicado;
    struct janela_botao botoes[JANELA_NUMERO_BOTOES];
    int numero_botoes;
    int tamanho_texto_botoes;
    int botao_selecionado;
};

struct janela janela_nova(int largura, int altura, char titulo[], char mensagem[]);

struct janela janela_adiciona_botao(struct janela janela,
    enum janela_tipo_botao tipo, char texto_botao[JANELA_TAMANHO_TEXTO_BOTAO]);

struct janela janela_atualiza(struct janela janela, int tecla);

struct janela janela_desenha(struct janela janela);

struct janela janela_mostrar(struct janela janela);

#endif /* COLLECTOR_JANELA_H */

