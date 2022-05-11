#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "utilitarios.h"
#include "telas.h"
#include "janela.h"

struct collector {
    enum telas tela;

    int redesenha;
    int sair_do_jogo;

    struct tela_inicial tela_inicial;
};

struct collector novo_collector(void);
void roda_collector(struct collector collector);

#endif /* COLLECTOR_H */

