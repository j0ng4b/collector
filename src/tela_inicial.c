#include <time.h>
#include <conio2.h>
#include "telas.h"
#include "collector.h"

struct tela_inicial tela_inicial_nova(void)
{
    struct tela_inicial tela = { 0 };

    tela.mostra_mensagem = 0;
    tela.temporizador = clock();

    return tela;
}

struct tela_inicial tela_inicial_atualiza(struct tela_inicial tela)
{
    struct contexto contexto = collector_contexto();

    if (contexto.tecla != 0) {
        contexto.tela = TELA_MENU;
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;
    }

    if ((clock() - tela.temporizador) / (double) CLOCKS_PER_SEC > 0.5) {
        tela.mostra_mensagem = !tela.mostra_mensagem;
        tela.temporizador = clock();

        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    }

    collector_altera_contexto(contexto);
    return tela;
}

struct tela_inicial tela_inicial_desenha(struct tela_inicial tela)
{
    desenha_moldura(1, 1, 1, COLUNAS, LINHAS);
    desenha_nome_jogo(METADE_COLUNAS - 36, METADE_LINHAS - 3);

    gotoxy(METADE_COLUNAS - 18, LINHAS * 0.8);

    if (tela.mostra_mensagem)
        cprintf("Precione qualquer tecla para iniciar!");
    else
        cprintf("                                     ");

    gotoxy(METADE_COLUNAS, LINHAS);

    return tela;
}

