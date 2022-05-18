#include <string.h>
#include <conio2.h>
#include "collector.h"
#include "utilitarios.h"

static struct contexto contexto_alterado;

static struct collector atualiza_telas(struct collector collector);
static struct collector desenha_telas(struct collector collector);
static struct collector atualiza_contexto(struct collector collector);
static struct collector verifica_novo_recorde(struct collector);

static struct collector animacao_comum(struct collector collector);
static void animacao_gameover(void);

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

    collector.animacao_pendente = 0;
    memset(collector.contexto.cores_fundo_animacao_normal, BLACK,
        sizeof(collector.contexto.cores_fundo_animacao_normal));

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
        if (collector.animacao_pendente) {
            if (collector.animacao_pendente == 1)
                collector = animacao_comum(collector);
            else if (collector.animacao_pendente == 2)
                animacao_gameover();

            while (kbhit()) getch();
        }

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
    collector.animacao_pendente = 0;

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
    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_TELA
        & ~COLLECTOR_CONTEXTO_REDESENHAR_TELA) {
        collector.contexto.tela = contexto_alterado.tela;

        if (collector.contexto.tela == TELA_GAMEOVER)
            collector.animacao_pendente = 2;
        else
            collector.animacao_pendente = 1;
    }

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

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_CORES_FUNDO_ANIMACAO) {
        memcpy(collector.contexto.cores_fundo_animacao_normal,
            contexto_alterado.cores_fundo_animacao_normal,
            sizeof(contexto_alterado.cores_fundo_animacao_normal));
    }

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

static struct collector animacao_comum(struct collector collector)
{
    char linha_vazia[COLUNAS + 1];
    int i, j;

    memset(linha_vazia, ' ', sizeof(linha_vazia));
    linha_vazia[COLUNAS] = '\0';

    textcolor(WHITE);

    for (i = 1; i <= METADE_LINHAS; ++i) {
        textbackground(WHITE);

        gotoxy(1, i);
        cprintf("%s", linha_vazia);

        gotoxy(1, LINHAS - (i - 1));
        cprintf("%s", linha_vazia);

        if (i > 1) {
            textbackground(collector.contexto.cores_fundo_animacao_normal[i - 2]);

            gotoxy(1, i - 1);
            cprintf("%s", linha_vazia);

            textbackground(collector.contexto.cores_fundo_animacao_normal[LINHAS - (i - 2) - 1]);
            gotoxy(1, LINHAS - (i - 2));
            cprintf("%s", linha_vazia);
        }

        gotoxy(METADE_COLUNAS, METADE_LINHAS);
        msleep(50);
    }

    textbackground(BLACK);
    for (j = METADE_COLUNAS; j > 0; --j) {
        for (i = 0; i < 2; ++i) {
            textbackground(collector.contexto.cores_fundo_animacao_normal[METADE_LINHAS + i]);
            gotoxy(j, METADE_LINHAS + i);
            cprintf(" ");

            gotoxy(COLUNAS - (j - 1), METADE_LINHAS + i);
            cprintf(" ");
        }

        gotoxy(METADE_COLUNAS, LINHAS);
        msleep(20);
    }

    memset(collector.contexto.cores_fundo_animacao_normal, BLACK,
        sizeof(collector.contexto.cores_fundo_animacao_normal));

    return collector;
}

static void animacao_gameover(void)
{
    int i;
    int bolas[COLUNAS + 1] = {0};

    textcolor(WHITE);
    while (bolas[COLUNAS] <= LINHAS) {
        for (i = 1; i <= COLUNAS; ++i) {
            if (bolas[i] > 0) {
                if (bolas[i] <= LINHAS) {
                    textbackground(WHITE);

                    gotoxy(i, bolas[i]);
                    cprintf(" ");

                    gotoxy(COLUNAS + 1 - i, bolas[i]);
                    cprintf(" ");
                }

                if (bolas[i] > 1) {
                    textbackground(BLACK);

                    gotoxy(i, bolas[i] - 1);
                    cprintf(" ");

                    gotoxy(COLUNAS + 1 - i, bolas[i] - 1);
                    cprintf(" ");
                }
            }

            if (i > 1)
                bolas[i] += bolas[i] <= LINHAS && bolas[i - 1] > 3;
            else
                bolas[i] += bolas[i] <= LINHAS;
        }

        gotoxy(COLUNAS / 2, LINHAS);
    }

    textbackground(BLACK);

    gotoxy(1, bolas[COLUNAS] - 1);
    cprintf(" ");

    gotoxy(COLUNAS, bolas[COLUNAS] - 1);
    cprintf(" ");
}

