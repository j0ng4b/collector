#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "utilitarios.h"
#include "telas.h"
#include "janela.h"

enum tipo_pedidos_collector {
    COLLECTOR_NENHUM_PEDIDO,
    COLLECTOR_MUDAR_TELA,
    COLLECTOR_REDESENHAR_TELA,
    COLLECTOR_SAIR_DO_JOGO,
};

struct collector {
    enum telas tela;

    int redesenha;
    int sair_do_jogo;

    struct tela_inicial tela_inicial;
};

struct pedido_colletor {
    enum telas tela;
    enum tipo_pedidos_collector pedido;
};

struct collector novo_collector(void);
void roda_collector(struct collector collector);

void pedir_collector(struct pedido_colletor pedido);

#endif /* COLLECTOR_H */

