#include <conio2.h>
#include "telas.h"
#include "collector.h"

#define TELA_MENU_NUMERO_OPCOES   3
#define TELA_MENU_NUMERO_LEGENDAS 3
#define TELA_MENU_TAMANHO_TEXTOS  30

struct tela_menu tela_menu_nova(void)
{
    struct tela_menu tela_menu;
    struct janela_cores cores_janela = { BLACK, WHITE };

    tela_menu.opcao_selecionada = 0;

    tela_menu.janela.sair = janela_nova(20, 10, "Deseja realmente sair?",
        "Se sim, sem problemas seus recordes não serão perdidos.",
        cores_janela);

    tela_menu.janela.sair = janela_adiciona_botao(tela_menu.janela.sair,
        JANELA_BOTAO_ACEITAR, "Sim");
    tela_menu.janela.sair = janela_adiciona_botao(tela_menu.janela.sair,
        JANELA_BOTAO_NEGAR, "Nao");

    tela_menu.janela.informacoes = janela_nova(57, 15, "Informacoes",
        "1. Ao iniciar o jogo as teclas responsaveis por mover"
        "   a barra (coletor) serao a e d.                    "
        "                                                     "
        "2. Quando o jogo for iniciado o objetivo sera nao    "
        "   deixar nenhum item cair no chao.                  "
        "                                                     "
        "3. Para contabilizar os pontos sera necessario guiar "
        "   a barra (coletor) ate o item para coleta-lo.      "
        "                                                     "
        "4. A cada certa pontuacao a dificuldade aumenta.     ",
        cores_janela);

    tela_menu.janela.informacoes = janela_adiciona_botao(tela_menu.janela.informacoes,
        JANELA_BOTAO_ACEITAR, "OK");

    return tela_menu;
}

struct tela_menu tela_menu_atualiza(struct tela_menu tela)
{
    int janela_botao_selecionado = tela.janela.sair.botao_selecionado;
    struct contexto contexto = collector_contexto();

    if (tela.janela.sair.visivel) {
        tela.janela.sair = janela_atualiza(tela.janela.sair, contexto.tecla);

        if (janela_botao_selecionado != tela.janela.sair.botao_selecionado)
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;

        switch (tela.janela.sair.botao_clicado) {
        case JANELA_BOTAO_ACEITAR:
            contexto.alteracao = COLLECTOR_CONTEXTO_SAIR_DO_JOGO;
            break;

        default:
            break;
        }
    } else if (tela.janela.informacoes.visivel) {
        tela.janela.informacoes = janela_atualiza(tela.janela.informacoes,
            contexto.tecla);

        if (tela.janela.sair.botao_clicado != JANELA_BOTAO_NULO)
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;

    } else if (contexto.tecla == '\r') {
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;

        switch (tela.opcao_selecionada) {
        case 0:
            contexto.tela = TELA_NIVEIS;
            break;

        case 1:
            tela.janela.informacoes = janela_mostrar(tela.janela.informacoes);
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
            break;

        case 2:
            tela.janela.sair = janela_mostrar(tela.janela.sair);
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
            break;
        }
    } else if (contexto.tecla == 'w' && tela.opcao_selecionada > 0) {
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
        tela.opcao_selecionada--;
    } else if (contexto.tecla == 's' && tela.opcao_selecionada < 2) {
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
        tela.opcao_selecionada++;
    }

    collector_altera_contexto(contexto);
    return tela;
}

struct tela_menu tela_menu_desenha(struct tela_menu tela)
{
    char legendas[TELA_MENU_NUMERO_LEGENDAS][TELA_MENU_TAMANHO_TEXTOS] = {
        "enter: confirmar selecao", "s: seta para baixo", "w: seta para cima",
    };

    char opcoes[TELA_MENU_NUMERO_OPCOES][TELA_MENU_TAMANHO_TEXTOS] = {
        "Iniciar", "Informacoes", "Sair",
    };

    int i;

    if (tela.janela.sair.visivel) {
        tela.janela.sair = janela_desenha(tela.janela.sair);
        return tela;
    } else if (tela.janela.informacoes.visivel) {
        tela.janela.informacoes = janela_desenha(tela.janela.informacoes);
        return tela;
    }

    textbackground(BLACK);
    clrscr();
    desenha_nome_jogo(METADE_COLUNAS - 36, LINHAS * 0.3);

    for (i = 0; i < TELA_MENU_NUMERO_OPCOES; ++i) {
        if (i == tela.opcao_selecionada)
            textbackground(RED);
        else
            textbackground(BLACK);

        gotoxy(METADE_COLUNAS - strlen(opcoes[i]) / 2, LINHAS * 0.3 + 8 + i);
        cprintf(" %s ", opcoes[i]);
    }

    textbackground(BLACK);
    for (i = 0; i < TELA_MENU_NUMERO_LEGENDAS; ++i) {
        gotoxy(1, LINHAS - i);
        cprintf("%s", legendas[i]);
    }

    return tela;
}

