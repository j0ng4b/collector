#ifndef COLLECTOR_JANELA_H
#define COLLECTOR_JANELA_H

#include "utilitarios.h"

#define JANELA_LARGURA_MAXIMA         (COLUNAS * 80 / 100)
#define JANELA_ALTURA_MAXIMA          (LINHAS * 80 / 100)
#define JANELA_NUMERO_BOTOES          3
#define JANELA_TAMANHO_TITULO         (JANELA_LARGURA_MAXIMA - 10)
#define JANELA_TAMANHO_MENSAGEM_LINHA (JANELA_LARGURA_MAXIMA - 4)
#define JANELA_TAMANHO_MENSAGEM       \
    ((JANELA_LARGURA_MAXIMA - 4) * (JANELA_ALTURA_MAXIMA - 4))

#define BOTAO_TAMANHO_TEXTO           10

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

/* nova_janela
 */
struct janela nova_janela(int largura, int altura, char titulo[], char mensagem[]);

/* adiciona_botao_janela
 */
struct janela adiciona_botao_janela(struct janela janela, enum tipo_botao tipo,
    char texto_botao[BOTAO_TAMANHO_TEXTO]);

/* desenha_janela
 */
struct janela desenha_janela(struct janela janela);

/* atualiza_janela
 */
struct janela atualiza_janela(struct janela janela, int tecla);

/* mostra_janela
 */
struct janela mostra_janela(struct janela janela);

#endif /* COLLECTOR_JANELA_H */

