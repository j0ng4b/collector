#include <conio2.h>
#include "collector.h"

struct collector novo_collector(void)
{
    struct collector collector;

    collector.tela = TELA_INICIAL;

    collector.redesenha = 1;
    collector.sair_do_jogo = 0;

    collector.tela_inicial = nova_tela_inicial();

    return collector;
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

    default:
        break;
    }

    collector.redesenha = 0;
    return collector;
}

void roda_collector(struct collector collector)
{
    while (!collector.sair_do_jogo) {
        collector = atualiza_telas(collector);
        collector = desenha_telas(collector);
    }
}

