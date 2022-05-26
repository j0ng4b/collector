#include <conio2.h>
#include "telas.h"
#include "collector.h"

#define TELA_MENU_NUMERO_OPCOES   4
#define TELA_MENU_NUMERO_LEGENDAS 3
#define TELA_MENU_TAMANHO_TEXTOS  30

void tela_menu_nova(struct tela_menu *tela)
{
    struct janela_config janela_sair_config = {
        { 20, 10 },
        {
            "Deseja realmente sair?",
            "Se sim, sem problemas, seus recordes nao serao perdidos.",

            0
        }, 0,
        { BLACK, WHITE },
        JANELA_COMUM
    };

    struct janela_config janela_informacoes_config = {
        { 57, 15 },
        {
            "Informacoes",
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

            1
        }, 0,
        { BLACK, WHITE },
        JANELA_COMUM
    };


    janela_nova(&tela->janela.sair, &janela_sair_config);

    janela_adiciona_botao(&tela->janela.sair, JANELA_BOTAO_ACEITAR, "Sim");
    janela_adiciona_botao(&tela->janela.sair, JANELA_BOTAO_NEGAR, "Nao");

    janela_nova(&tela->janela.informacoes, &janela_informacoes_config);

    janela_adiciona_botao(&tela->janela.informacoes, JANELA_BOTAO_ACEITAR, "OK");

    tela->opcao_selecionada = 0;
}

void tela_menu_atualiza(struct tela_menu *tela)
{
    struct contexto contexto = collector_contexto();

    if (tela->janela.sair.visivel) {
        switch (janela_atualiza(&tela->janela.sair, contexto.tecla)) {
        case JANELA_BOTAO_ACEITAR:
            contexto.alteracao = COLLECTOR_CONTEXTO_SAIR_DO_JOGO;
            break;

        case JANELA_BOTAO_NEGAR:
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
            break;

        default:
            break;
        }

        if (tela->janela.sair.redesenhar) {
            contexto.numero_audio = 0;
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA
                | COLLECTOR_CONTEXTO_TOCAR_AUDIO;
        }
    } else if (tela->janela.informacoes.visivel) {
        if (janela_atualiza(&tela->janela.informacoes, contexto.tecla)
            != JANELA_BOTAO_NULO)
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;

    } else if (contexto.tecla == '\r') {
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;

        switch (tela->opcao_selecionada) {
        case 0:
            contexto.tela = TELA_NIVEIS;
            break;

        case 1:
            janela_mostrar(&tela->janela.informacoes);
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
            break;

        case 2:
            contexto.tela = TELA_CREDITOS;
            break;

        case 3:
            janela_mostrar(&tela->janela.sair);
            contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
            break;
        }
    } else if (contexto.tecla == 'w' && tela->opcao_selecionada > 0) {
        tela->opcao_selecionada--;

        contexto.numero_audio = 0;
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA
            | COLLECTOR_CONTEXTO_TOCAR_AUDIO;
    } else if (contexto.tecla == 's'
        && tela->opcao_selecionada < TELA_MENU_NUMERO_OPCOES - 1) {
        tela->opcao_selecionada++;

        contexto.numero_audio = 0;
        contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA
            | COLLECTOR_CONTEXTO_TOCAR_AUDIO;
    }

    collector_altera_contexto(&contexto);
}

void tela_menu_desenha(struct tela_menu *tela)
{
    char legendas[TELA_MENU_NUMERO_LEGENDAS][TELA_MENU_TAMANHO_TEXTOS] = {
        "enter: confirmar selecao", "s: seta para baixo", "w: seta para cima",
    };

    char opcoes[TELA_MENU_NUMERO_OPCOES][TELA_MENU_TAMANHO_TEXTOS] = {
        "Iniciar", "Informacoes", "Creditos", "Sair",
    };

    int i;

    if (tela->janela.sair.visivel) {
        janela_desenha(&tela->janela.sair);
        return;
    } else if (tela->janela.informacoes.visivel) {
        janela_desenha(&tela->janela.informacoes);
        return;
    }

    textbackground(BLACK);
    clrscr();
    desenha_nome_jogo(METADE_COLUNAS - 36, LINHAS * 0.3);

    for (i = 0; i < TELA_MENU_NUMERO_OPCOES; ++i) {
        if (i == tela->opcao_selecionada)
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
}

