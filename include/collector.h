#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "telas.h"

#define RECORDE_MAXIMO_TEXTO_LINHA 43
#define RECORDE_TAMANHO_NOME       11
#define RECORDE_QUANTIDADE         3

enum collector_tipo_alteracao_contexto {
    COLLECTOR_CONTEXTO_REDESENHAR_TELA = 0x00000010,

    COLLECTOR_CONTEXTO_ALTERAR_TELA = (COLLECTOR_CONTEXTO_REDESENHAR_TELA | 0x00000002),
    COLLECTOR_CONTEXTO_ALTERAR_NIVEL = 0x00000004,
    COLLECTOR_CONTEXTO_ALTERAR_PONTOS = 0x00000008,
    COLLECTOR_CONTEXTO_ALTERAR_RECORDE = 0x0000001,

    COLLECTOR_CONTEXTO_SAIR_DO_JOGO  = 0x10000000,
    COLLECTOR_CONTEXTO_SEM_ALTERACAO = 0x00000000,
};

struct contexto {
    enum telas tela;

    int nivel_dificuldade;
    int pontos;

    int redesenhar;
    int sair_do_jogo;

    int tecla;

    struct {
        char nome[RECORDE_TAMANHO_NOME];
        int pontos;
    } recordes[RECORDE_QUANTIDADE];
    int novo_recorde;

    enum collector_tipo_alteracao_contexto alteracao;
};

struct collector {
    struct contexto contexto;

    int animacao_pendente;

    struct {
        struct tela_inicial  inicial;
        struct tela_menu     menu;
        struct tela_niveis   niveis;
        struct tela_jogo     jogo;
        struct tela_gameover gameover;
    } tela;
};

/* collector_novo
 */
struct collector collector_novo(void);

/* collector_rodar
 */
void collector_rodar(struct collector collector);

/* collector_altera_contexto
 */
void collector_altera_contexto(struct contexto contexto);

/* collector_contexto
 */
struct contexto collector_contexto(void);

#endif /* COLLECTOR_H */

