#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "utilitarios.h"
#include "janela.h"
#include "telas.h"

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
    struct tela_menu tela_menu;
    struct tela_jogo tela_jogo;
};

struct pedido_colletor {
    enum telas tela;
    enum tipo_pedidos_collector pedido;
};

/* novo_collector
 */
struct collector novo_collector(void);

/* roda_collector
 */
void roda_collector(struct collector collector);

/* pedir_collector
 */
void pedir_collector(struct pedido_colletor pedido);

#endif /* COLLECTOR_H */

