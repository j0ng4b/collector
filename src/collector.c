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

    return collector;
}

void roda_collector(struct collector collector)
{
    while (!collector.sair_do_jogo) {
        collector = processa_pedidos(collector);
        collector = atualiza_telas(collector);
        collector = desenha_telas(collector);
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
        break;

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

