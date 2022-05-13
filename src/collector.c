#include <conio2.h>
#include "collector.h"

static struct pedido_colletor pedido_pendente;

static struct collector atualiza_telas(struct collector collector);
static struct collector desenha_telas(struct collector collector);
static struct collector processa_pedidos(struct collector collector);

struct collector novo_collector(void)
{
    struct collector collector;

    collector.tela = TELA_INICIAL;

    collector.redesenha = 1;
    collector.sair_do_jogo = 0;

    collector.tela_inicial = nova_tela_inicial();
    collector.tela_menu = nova_tela_menu();
    collector.tela_jogo = nova_tela_jogo();

    return collector;
}

void roda_collector(struct collector collector)
{
    while (!collector.sair_do_jogo) {
        collector = processa_pedidos(collector);
        collector = atualiza_telas(collector);
        collector = desenha_telas(collector);

        gotoxy(METADE_COLUNAS, LINHAS);
    }
}

void pedir_collector(struct pedido_colletor pedido)
{
    pedido_pendente = pedido;
}

static struct collector atualiza_telas(struct collector collector)
{
    int tecla = 0;

    if (kbhit())
        tecla = getch();

    if (tecla == 'q')
        collector.sair_do_jogo = 1;

    switch (collector.tela) {
    case TELA_INICIAL:
        collector.tela_inicial = atualiza_tela_inicial(collector.tela_inicial,
            tecla);
        break;

    case TELA_MENU:
        collector.tela_menu = atualiza_tela_menu(collector.tela_menu, tecla);
        break;

    case TELA_JOGO:
        collector.tela_jogo = atualiza_tela_jogo(collector.tela_jogo, tecla);
        break;

    default:
        break;
    }

    return collector;
}

static struct collector desenha_telas(struct collector collector)
{
    if (!collector.redesenha)
        return collector;

    switch (collector.tela) {
    case TELA_INICIAL:
        collector.tela_inicial = desenha_tela_inicial(collector.tela_inicial);
        break;

    case TELA_MENU:
        collector.tela_menu = desenha_tela_menu(collector.tela_menu);
        break;

    case TELA_JOGO:
        collector.tela_jogo = desenha_tela_jogo(collector.tela_jogo);
        break;

    default:
        break;
    }

    collector.redesenha = 0;
    return collector;
}

static struct collector processa_pedidos(struct collector collector)
{
    switch (pedido_pendente.pedido) {
    case COLLECTOR_MUDAR_TELA:
        collector.tela = pedido_pendente.tela;

    case COLLECTOR_REDESENHAR_TELA:
        collector.redesenha = 1;
        break;

    case COLLECTOR_SAIR_DO_JOGO:
        collector.sair_do_jogo = 1;
        break;

    default:
        break;
    }

    pedido_pendente.pedido = COLLECTOR_NENHUM_PEDIDO;
    return collector;
}

