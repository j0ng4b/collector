#include <time.h>
#include <conio2.h>
#include "collector.h"

struct tela_inicial nova_tela_inicial(void)
{
    struct tela_inicial tela_inicial;

    tela_inicial.mostra_mensagem = 0;
    tela_inicial.temporizador = clock();

    return tela_inicial;
}

struct tela_inicial atualiza_tela_inicial(struct tela_inicial tela_inicial,
    int tecla)
{
    struct pedido_collector pedido;

    if (tecla != 0) {
        pedido.tela = TELA_MENU;
        pedido.pedido = COLLECTOR_MUDAR_TELA;
    }

    if ((clock() - tela_inicial.temporizador) / (double) CLOCKS_PER_SEC > 0.5) {
        tela_inicial.mostra_mensagem = !tela_inicial.mostra_mensagem;
        tela_inicial.temporizador = clock();
        pedido.pedido = COLLECTOR_REDESENHAR_TELA;
    }

    pedir_collector(pedido);
    return tela_inicial;
}

struct tela_inicial desenha_tela_inicial(struct tela_inicial tela_inicial)
{
    desenha_moldura(1, 1, 1, COLUNAS, LINHAS);
    desenha_nome_jogo(METADE_COLUNAS - 36, METADE_LINHAS - 3);

    gotoxy(METADE_COLUNAS - 18, LINHAS * 0.8);

    if (tela_inicial.mostra_mensagem)
        cprintf("Precione qualquer tecla para iniciar!");
    else
        cprintf("                                     ");

    gotoxy(METADE_COLUNAS, LINHAS);

    return tela_inicial;
}

