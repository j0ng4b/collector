#include <conio2.h>
#include "collector.h"

#define TELA_MENU_NUMERO_OPCOES   3
#define TELA_MENU_NUMERO_LEGENDAS 3
#define TELA_MENU_TAMANHO_TEXTOS  30

struct tela_menu nova_tela_menu(void)
{
    struct tela_menu tela_menu;

    tela_menu.opcao_selecionada = 0;
    tela_menu.janela_sair = nova_janela(20, 10, "Deseja realmente sair?",
        "Caso saia do jogo todo seu progresso sera perdido!");

    tela_menu.janela_sair = adiciona_botao_janela(tela_menu.janela_sair,
        BOTAO_ACEITAR, "Sim");
    tela_menu.janela_sair = adiciona_botao_janela(tela_menu.janela_sair,
        BOTAO_NEGAR, "Nao");

    return tela_menu;
}

struct tela_menu atualiza_tela_menu(struct tela_menu tela_menu, int tecla)
{
    int janela_botao_selecionado = tela_menu.janela_sair.botao_selecionado;
    struct pedido_colletor pedido = { 0 };

    if (tela_menu.janela_sair.visivel) {
        tela_menu.janela_sair = atualiza_janela(tela_menu.janela_sair, tecla);

        if (janela_botao_selecionado != tela_menu.janela_sair.botao_selecionado)
            pedido.pedido = COLLECTOR_REDESENHAR_TELA;

        switch (tela_menu.janela_sair.botao_clicado) {
        case BOTAO_ACEITAR:
            pedido.pedido = COLLECTOR_SAIR_DO_JOGO;
            break;

        default:
            break;
        }
    } else if (tecla == '\r') {
        pedido.pedido = COLLECTOR_MUDAR_TELA;

        switch (tela_menu.opcao_selecionada) {
        case 0:
            pedido.tela = TELA_DIFICULDADES;
            break;

        case 1:
            pedido.tela = TELA_REGRAS;
            break;

        case 2:
            tela_menu.janela_sair = mostra_janela(tela_menu.janela_sair);
            pedido.pedido = COLLECTOR_REDESENHAR_TELA;
            break;
        }
    } else if (tecla == 'w' && tela_menu.opcao_selecionada > 0) {
        pedido.pedido = COLLECTOR_REDESENHAR_TELA;
        tela_menu.opcao_selecionada--;
    } else if (tecla == 's' && tela_menu.opcao_selecionada < 2) {
        pedido.pedido = COLLECTOR_REDESENHAR_TELA;
        tela_menu.opcao_selecionada++;
    }

    pedir_collector(pedido);
    return tela_menu;
}

struct tela_menu desenha_tela_menu(struct tela_menu tela_menu)
{
    char legendas[TELA_MENU_NUMERO_LEGENDAS][TELA_MENU_TAMANHO_TEXTOS] = {
        "enter: confirmar selecao", "s: seta para baixo", "w: seta para cima",
    };

    char opcoes[TELA_MENU_NUMERO_OPCOES][TELA_MENU_TAMANHO_TEXTOS] = {
        "Iniciar", "Regras", "Sair",
    };

    int i;

    if (tela_menu.janela_sair.visivel) {
        tela_menu.janela_sair = desenha_janela(tela_menu.janela_sair);
        return tela_menu;
    }

    clrscr();
    textcolor(WHITE);
    desenha_nome_jogo(METADE_COLUNAS - 36, LINHAS * 0.3);

    for (i = 0; i < TELA_MENU_NUMERO_OPCOES; ++i) {
        if (i == tela_menu.opcao_selecionada)
            textcolor(RED);
        else
            textcolor(WHITE);

        gotoxy(METADE_COLUNAS - strlen(opcoes[i]) / 2, LINHAS * 0.3 + 8 + i);
        cprintf("%s", opcoes[i]);
    }

    textcolor(WHITE);
    for (i = 0; i < TELA_MENU_NUMERO_LEGENDAS; ++i) {
        gotoxy(1, LINHAS - i);
        cprintf("%s", legendas[i]);
    }

    return tela_menu;
}

