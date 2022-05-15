#include <string.h>
#include <conio2.h>
#include "collector.h"

static struct contexto contexto_alterado;

static struct collector atualiza_telas(struct collector collector);
static struct collector desenha_telas(struct collector collector);
static struct collector atualiza_contexto(struct collector collector);
static struct collector verifica_novo_recorde(struct collector);

struct collector collector_novo(void)
{
    int i;
    struct collector collector;

    collector.contexto.tela = TELA_INICIAL;

    collector.contexto.nivel_dificuldade = 0;
    collector.contexto.pontos = 0;

    collector.contexto.redesenhar = 1;
    collector.contexto.sair_do_jogo = 0;

    collector.contexto.novo_recorde = 0;
    memset(collector.contexto.recordes, 0, sizeof(collector.contexto.recordes));
    for (i = 0; i < RECORDE_QUANTIDADE; ++i)
        memset(collector.contexto.recordes[i].nome, 'A',
            sizeof(collector.contexto.recordes[i].nome) - 1);

    collector.contexto.alteracao = COLLECTOR_CONTEXTO_SEM_ALTERACAO;

    collector.tela.inicial = tela_inicial_nova();
    collector.tela.menu = tela_menu_nova();
    collector.tela.niveis = tela_niveis_nova();
    collector.tela.jogo = tela_jogo_nova();
    collector.tela.gameover = tela_gameover_nova();

    return collector;
}

void collector_rodar(struct collector collector)
{
    while (!collector.contexto.sair_do_jogo) {
        collector = atualiza_telas(collector);
        collector = desenha_telas(collector);
        collector = atualiza_contexto(collector);

        gotoxy(METADE_COLUNAS, LINHAS);
    }
}

void collector_altera_contexto(struct contexto contexto)
{
    contexto_alterado = contexto;
}

struct contexto collector_contexto(void)
{
    return contexto_alterado;
}

static struct collector atualiza_telas(struct collector collector)
{
    collector.contexto.tecla = 0;

    if (kbhit())
        collector.contexto.tecla = getch();

    if (collector.contexto.tecla == 'q')
        collector.contexto.sair_do_jogo = 1;

    contexto_alterado = collector.contexto;

    switch (collector.contexto.tela) {
    case TELA_INICIAL:
        collector.tela.inicial = tela_inicial_atualiza(collector.tela.inicial);
        break;

    case TELA_MENU:
        collector.tela.menu = tela_menu_atualiza(collector.tela.menu);
        break;

    case TELA_NIVEIS:
        collector.tela.niveis = tela_niveis_atualiza(collector.tela.niveis);
        break;

    case TELA_JOGO:
        collector.tela.jogo = tela_jogo_atualiza(collector.tela.jogo);
        break;

    case TELA_GAMEOVER:
        collector.tela.gameover = tela_gameover_atualiza(collector.tela.gameover);
        break;

    default:
        break;
    }

    return collector;
}

static struct collector desenha_telas(struct collector collector)
{
    if (!collector.contexto.redesenhar)
        return collector;

    switch (collector.contexto.tela) {
    case TELA_INICIAL:
        collector.tela.inicial = tela_inicial_desenha(collector.tela.inicial);
        break;

    case TELA_MENU:
        collector.tela.menu = tela_menu_desenha(collector.tela.menu);
        break;

    case TELA_NIVEIS:
        collector.tela.niveis = tela_niveis_desenha(collector.tela.niveis);
        break;

    case TELA_JOGO:
        collector.tela.jogo = tela_jogo_desenha(collector.tela.jogo);
        break;

    case TELA_GAMEOVER:
        collector.tela.gameover = tela_gameover_desenha(collector.tela.gameover);
        break;

    default:
        break;
    }

    collector.contexto.redesenhar = 0;
    return collector;
}

static struct collector atualiza_contexto(struct collector collector)
{
    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_TELA)
        collector.contexto.tela = contexto_alterado.tela;

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_REDESENHAR_TELA)
        collector.contexto.redesenhar = 1;

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_NIVEL)
        collector.contexto.nivel_dificuldade = contexto_alterado.nivel_dificuldade;

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_PONTOS) {
        collector.contexto.pontos = contexto_alterado.pontos;
        collector = verifica_novo_recorde(collector);
    }

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_RECORDE)
        strcpy(collector.contexto.recordes[contexto_alterado.novo_recorde - 1].nome,
            contexto_alterado.recordes[contexto_alterado.novo_recorde - 1].nome);

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_SAIR_DO_JOGO)
        collector.contexto.sair_do_jogo = 1;

    return collector;
}

/* verifica_novo_recorde
 *
 * Função para verificar se o jogador conseguiu atingir uma nova pontuação
 * máxima. Se ele conseguiu atribuí essa nova pontuação na lista de recordes.
 */
static struct collector verifica_novo_recorde(struct collector collector)
{
    int i, j;

    if (collector.contexto.pontos == 0) {
        collector.contexto.novo_recorde = 0;
        return collector;
    }

    for (i = 0; i < RECORDE_QUANTIDADE; ++i) {
        if (collector.contexto.recordes[i].pontos <= collector.contexto.pontos) {
            /* Move todos os recordes uma posição para baixo. Na posição do novo
             * recorde a nova pontuação é atribuída e o antigo nome é
             * substituído.
             */
            for (j = RECORDE_QUANTIDADE - 1; j >= i; --j) {
                collector.contexto.recordes[j].pontos = j == i ?
                    collector.contexto.pontos :
                    collector.contexto.recordes[j - 1].pontos;

                if (j != i)
                    strcpy(collector.contexto.recordes[j].nome,
                        collector.contexto.recordes[j - 1].nome);
                else
                    memset(collector.contexto.recordes[j].nome, 'A',
                        sizeof(collector.contexto.recordes[j].nome) - 1);
            }

            collector.contexto.novo_recorde = i + 1;
            return collector;
        }
    }

    collector.contexto.novo_recorde = 0;
    return collector;
}

