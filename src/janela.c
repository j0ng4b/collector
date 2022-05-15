#include <string.h>
#include <conio2.h>
#include "janela.h"

struct janela janela_nova(int largura, int altura, char titulo[], char mensagem[])
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

struct janela janela_adiciona_botao(struct janela janela,
    enum janela_tipo_botao tipo, char texto_botao[JANELA_TAMANHO_TEXTO_BOTAO])
{
    int i = 0;
    int tamanho_texto_botoes;
    struct janela_botao botao = { 0 };

    botao.tamanho_texto = strlen(texto_botao);
    if (botao.tamanho_texto >= JANELA_TAMANHO_TEXTO_BOTAO)
        return (struct janela) { 0 };

    strncpy(botao.texto, texto_botao, JANELA_TAMANHO_TEXTO_BOTAO);
    botao.tipo = tipo;
    botao.y = janela.altura - 2;

    janela.botoes[janela.numero_botoes] = botao;

    janela.tamanho_texto_botoes += botao.tamanho_texto + 2;
    janela.tamanho_texto_botoes += janela.numero_botoes > 0;

    janela.numero_botoes++;

    tamanho_texto_botoes = janela.tamanho_texto_botoes;
    janela.botoes[0].x = janela.largura / 2 - tamanho_texto_botoes / 2;
    for (i = 1; i < janela.numero_botoes; ++i)
        janela.botoes[i].x = janela.botoes[i - 1].x + janela.botoes[i - 1].tamanho_texto + 2;

    return janela;
}

struct janela janela_atualiza(struct janela janela, int tecla)
{
    if (!janela.visivel)
        return janela;

    janela.botao_clicado = JANELA_BOTAO_NULO;

    if (tecla == 'a' && janela.botao_selecionado > 0) {
        janela.botao_selecionado--;
        janela.redesenhar = 1;
    } else if (tecla == 'd' && janela.botao_selecionado + 1 < janela.numero_botoes) {
        janela.botao_selecionado++;
        janela.redesenhar = 1;
    } else if (tecla == '\r') {
        janela.visivel = 0;
        janela.botao_clicado = janela.botoes[janela.botao_selecionado].tipo;
        janela.botao_selecionado = 0;
    }

    return janela;
}

struct janela janela_desenha(struct janela janela)
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
        cprintf(" %s ", janela.botoes[x].texto);
    }

    textbackground(BLACK);
    gotoxy(janela.x + 2, janela.y);
    cprintf(" %s ", janela.titulo);

    return janela;
}

struct janela janela_mostrar(struct janela janela)
{
    janela.visivel = 1;
    janela.redesenhar = 1;

    return janela;
}

