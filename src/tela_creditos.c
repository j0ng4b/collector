#include <string.h>
#include <time.h>
#include <conio2.h>
#include "telas.h"
#include "collector.h"
#include "utilitarios.h"

void tela_creditos_nova(struct tela_creditos *tela)
{
    int i, j, k;

    struct {
        char texto[TELA_CREDITOS_TAMANHO_TEXTOS];
        int quantidade_nomes;
    } dados[] = {
        { "Programadores", 2 },
        { "Jonatha Gabriel Nascimento Santos", 0 },
        { "Igor Andrade Nepomuceno", 0, },

        { "Documentacao", 1 },
        { "Felipe Aragao Santos", 0 },

        { "Musica", 2 },
        { "Igor Andrade Nepomuceno", 0, },
        { "Blackaguia1 (SoundTrack)", 0 },

        { "Design", 1 },
        { "Jonatha Gabriel Nascimento Santos", 0 },

        { "Departamento de Sistemas de Informacao", 2 },
        { "Programacao I - 2021.2", 0 },
        { "Itabaiana - SE", 0 },
    };

    for (i = 0, j = 0; i < TELA_CREDITOS_QUANTIDADE; ++i) {
        strcpy(tela->creditos[i].titulo.texto, dados[j].texto);
        tela->creditos[i].titulo.tamanho = strlen(dados[j].texto);
        tela->creditos[i].quantidade_nomes = dados[j].quantidade_nomes;

        if (i > 0)
            tela->creditos[i].posicao_y = tela->creditos[i - 1].posicao_y +
                tela->creditos[i - 1].quantidade_nomes + 4;
        else
            tela->creditos[i].posicao_y = LINHAS;

        for (k = 0; k < dados[j].quantidade_nomes; ++k) {
            strcpy(tela->creditos[i].nomes[k].texto, dados[j + k + 1].texto);
            tela->creditos[i].nomes[k].tamanho = strlen(dados[j + k + 1].texto);
        }

        j += k + 1;
    }

    tela->temporizador = clock();
}

void tela_creditos_atualiza(struct tela_creditos *tela)
{
    int i;
    struct contexto contexto = collector_contexto();

    if (contexto.tecla == '\r') {
        tela->tempo = 0.1;
    } else if (contexto.tecla != 0) {
        contexto.tela = TELA_MENU;
        contexto.alteracao = COLLECTOR_CONTEXTO_ALTERAR_TELA;

        for (i = 0; i < TELA_CREDITOS_QUANTIDADE; ++i)
            if (i > 0)
                tela->creditos[i].posicao_y = tela->creditos[i - 1].posicao_y +
                    tela->creditos[i - 1].quantidade_nomes + 4;
            else
                tela->creditos[i].posicao_y = LINHAS;
    } else {
        tela->tempo = 0.7;
    }

    if ((clock() - tela->temporizador) / (double) CLOCKS_PER_SEC < tela->tempo) {
        collector_altera_contexto(&contexto);
        return;
    }

    tela->temporizador = clock();
    for (i = 0; i < TELA_CREDITOS_QUANTIDADE - 1; ++i)
        if (tela->creditos[i].posicao_y + tela->creditos[i].quantidade_nomes > 9)
            tela->creditos[i].posicao_y--;

    contexto.alteracao = COLLECTOR_CONTEXTO_REDESENHAR_TELA;
    collector_altera_contexto(&contexto);
}

void tela_creditos_desenha(struct tela_creditos *tela)
{
    int i, j;
    char linha_vazia[COLUNAS - 1];

    desenha_moldura(1, 1, 1, COLUNAS, LINHAS);
    desenha_nome_jogo(METADE_COLUNAS - 36, 2);

    memset(linha_vazia, ' ', sizeof(linha_vazia));
    linha_vazia[COLUNAS - 2] = '\0';
    for (i = 10; i < LINHAS - 1; ++i) {
        gotoxy(2, i);
        cprintf("%s", linha_vazia);
    }

    for (i = 0; i < TELA_CREDITOS_QUANTIDADE - 1; ++i) {
        textbackground(RED);
        if (tela->creditos[i].posicao_y > 9
            && tela->creditos[i].posicao_y < LINHAS - 1) {
            gotoxy(COLUNAS / 2 - tela->creditos[i].titulo.tamanho / 2,
                tela->creditos[i].posicao_y);
            cprintf(" %s ", tela->creditos[i].titulo.texto);
        }

        textbackground(BLACK);
        for (j = 0; j < tela->creditos[i].quantidade_nomes; ++j) {
            if (tela->creditos[i].posicao_y + j + 1 < 10
                || tela->creditos[i].posicao_y + j + 1 > LINHAS - 2)
                continue;

            gotoxy(COLUNAS / 2 - tela->creditos[i].nomes[j].tamanho / 2,
                tela->creditos[i].posicao_y + j + 1);
            cprintf("%s", tela->creditos[i].nomes[j].texto);
        }
    }

    if (tela->creditos[TELA_CREDITOS_QUANTIDADE - 2].posicao_y +
        tela->creditos[TELA_CREDITOS_QUANTIDADE - 2].quantidade_nomes < 10) {
        textbackground(BLACK);

        j = 8 + (LINHAS - 8) / 2 -
            (tela->creditos[TELA_CREDITOS_QUANTIDADE - 1].quantidade_nomes + 1) / 2;

        gotoxy(COLUNAS / 2 -
            tela->creditos[TELA_CREDITOS_QUANTIDADE - 1].titulo.tamanho / 2, j);
        cprintf(" %s ", tela->creditos[TELA_CREDITOS_QUANTIDADE - 1].titulo.texto);

        for (i = 0; i < tela->creditos[TELA_CREDITOS_QUANTIDADE - 1].quantidade_nomes; ++i) {
            gotoxy(COLUNAS / 2 - tela->creditos[TELA_CREDITOS_QUANTIDADE - 1]
                .nomes[i].tamanho / 2, j + i + 1);
            cprintf("%s", tela->creditos[TELA_CREDITOS_QUANTIDADE - 1]
                .nomes[i].texto);
        }
    }

    gotoxy(METADE_COLUNAS, LINHAS);
}

