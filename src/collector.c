#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include "collector.h"
#include "utilitarios.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static struct contexto contexto_alterado;

static void atualiza_telas(struct collector *collector);
static void desenha_telas(struct collector *collector);
static void atualiza_contexto(struct collector *collector);
static void verifica_novo_recorde(struct collector *collector);

static void carrega_recorde_salvo(struct collector *collector);
static void salva_recordes(struct collector *collector);

static void animacao_comum(struct collector *collector);
static void animacao_gameover(struct collector *collector);

static void inicializa_audio(struct collector *collector);

void collector_novo(struct collector *collector)
{
    int i;

    collector->contexto.tela = TELA_INICIAL;

    collector->contexto.nivel_dificuldade = 0;
    collector->contexto.pontos = 0;

    collector->contexto.redesenhar = 1;
    collector->contexto.sair_do_jogo = 0;

    collector->contexto.novo_recorde = 0;
    memset(collector->contexto.recordes, 0, sizeof(collector->contexto.recordes));
    for (i = 0; i < RECORDE_QUANTIDADE; ++i)
        memset(collector->contexto.recordes[i].nome, 'A',
            sizeof(collector->contexto.recordes[i].nome) - 1);

    collector->contexto.alteracao = COLLECTOR_CONTEXTO_SEM_ALTERACAO;

    collector->animacao_pendente = 0;
    memset(collector->contexto.cores_fundo_animacao_normal, BLACK,
        sizeof(collector->contexto.cores_fundo_animacao_normal));

    tela_inicial_nova(&collector->tela.inicial);
    tela_menu_nova(&collector->tela.menu);
    tela_niveis_nova(&collector->tela.niveis);
    tela_jogo_nova(&collector->tela.jogo);
    tela_gameover_nova(&collector->tela.gameover);
    tela_creditos_nova(&collector->tela.creditos);

    carrega_recorde_salvo(collector);
    inicializa_audio(collector);
}

void collector_rodar(struct collector *collector)
{
    while (!collector->contexto.sair_do_jogo) {
        if (collector->animacao_pendente) {
            if (collector->animacao_pendente == 1)
                animacao_comum(collector);
            else if (collector->animacao_pendente == 2)
                animacao_gameover(collector);

            while (kbhit()) getch();
        }

        atualiza_telas(collector);
        desenha_telas(collector);
        atualiza_contexto(collector);

        gotoxy(METADE_COLUNAS, LINHAS);
    }

    salva_recordes(collector);

    ma_decoder_uninit(&collector->audio.fundo);
    ma_device_uninit(&collector->audio.dispositivo);
}

void collector_altera_contexto(struct contexto *contexto)
{
    contexto_alterado = *contexto;
}

struct contexto collector_contexto(void)
{
    return contexto_alterado;
}

static void atualiza_telas(struct collector *collector)
{
    collector->contexto.tecla = 0;
    collector->animacao_pendente = 0;

    if (kbhit())
        collector->contexto.tecla = getch();

#ifdef DEBUG
    if (collector->contexto.tecla == 3)
        collector->contexto.sair_do_jogo = 1;
#endif /* DEBUG */

    contexto_alterado = collector->contexto;

    switch (collector->contexto.tela) {
    case TELA_INICIAL:
        tela_inicial_atualiza(&collector->tela.inicial);
        break;

    case TELA_MENU:
        tela_menu_atualiza(&collector->tela.menu);
        break;

    case TELA_NIVEIS:
        tela_niveis_atualiza(&collector->tela.niveis);
        break;

    case TELA_JOGO:
        tela_jogo_atualiza(&collector->tela.jogo);
        break;

    case TELA_GAMEOVER:
        tela_gameover_atualiza(&collector->tela.gameover);
        break;

    case TELA_CREDITOS:
        tela_creditos_atualiza(&collector->tela.creditos);
        break;

    default:
        break;
    }
}

static void desenha_telas(struct collector *collector)
{
    if (!collector->contexto.redesenhar)
        return;

    switch (collector->contexto.tela) {
    case TELA_INICIAL:
        tela_inicial_desenha(&collector->tela.inicial);
        break;

    case TELA_MENU:
        tela_menu_desenha(&collector->tela.menu);
        break;

    case TELA_NIVEIS:
        tela_niveis_desenha(&collector->tela.niveis);
        break;

    case TELA_JOGO:
        tela_jogo_desenha(&collector->tela.jogo);
        break;

    case TELA_GAMEOVER:
        tela_gameover_desenha(&collector->tela.gameover);
        break;

    case TELA_CREDITOS:
        tela_creditos_desenha(&collector->tela.creditos);
        break;

    default:
        break;
    }

    collector->contexto.redesenhar = 0;
}

static void atualiza_contexto(struct collector *collector)
{
    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_TELA
        & ~COLLECTOR_CONTEXTO_REDESENHAR_TELA) {
        collector->contexto.tela = contexto_alterado.tela;

        if (collector->contexto.tela == TELA_GAMEOVER)
            collector->animacao_pendente = 2;
        else
            collector->animacao_pendente = 1;
    }

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_REDESENHAR_TELA)
        collector->contexto.redesenhar = 1;

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_NIVEL)
        collector->contexto.nivel_dificuldade = contexto_alterado.nivel_dificuldade;

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_PONTOS) {
        collector->contexto.pontos = contexto_alterado.pontos;
        verifica_novo_recorde(collector);
    }

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_ALTERAR_RECORDE)
        strcpy(collector->contexto.recordes[contexto_alterado.novo_recorde - 1].nome,
            contexto_alterado.recordes[contexto_alterado.novo_recorde - 1].nome);

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_SAIR_DO_JOGO)
        collector->contexto.sair_do_jogo = 1;

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_CORES_FUNDO_ANIMACAO) {
        memcpy(collector->contexto.cores_fundo_animacao_normal,
            contexto_alterado.cores_fundo_animacao_normal,
            sizeof(contexto_alterado.cores_fundo_animacao_normal));
    }

    if (contexto_alterado.alteracao & COLLECTOR_CONTEXTO_TOCAR_AUDIO) {
        collector->audio.lista[contexto_alterado.numero_audio].finalizado = 0;
        ma_data_source_seek_to_pcm_frame(
            &collector->audio.lista[contexto_alterado.numero_audio].audio, 0);
    }
}

/* verifica_novo_recorde
 *
 * Função para verificar se o jogador conseguiu atingir uma nova pontuação
 * máxima. Se ele conseguiu atribuí essa nova pontuação na lista de recordes.
 */
static void verifica_novo_recorde(struct collector *collector)
{
    int i, j;

    if (collector->contexto.pontos == 0) {
        collector->contexto.novo_recorde = 0;
        return;
    }

    for (i = 0; i < RECORDE_QUANTIDADE; ++i) {
        if (collector->contexto.recordes[i].pontos <= collector->contexto.pontos) {
            /* Move todos os recordes uma posição para baixo. Na posição do novo
             * recorde a nova pontuação é atribuída e o antigo nome é
             * substituído.
             */
            for (j = RECORDE_QUANTIDADE - 1; j >= i; --j) {
                collector->contexto.recordes[j].pontos = j == i ?
                    collector->contexto.pontos :
                    collector->contexto.recordes[j - 1].pontos;

                if (j != i)
                    strcpy(collector->contexto.recordes[j].nome,
                        collector->contexto.recordes[j - 1].nome);
                else
                    memset(collector->contexto.recordes[j].nome, 'A',
                        sizeof(collector->contexto.recordes[j].nome) - 1);
            }

            collector->contexto.novo_recorde = i + 1;
            return;
        }
    }

    collector->contexto.novo_recorde = 0;
    return;
}

static void carrega_recorde_salvo(struct collector *collector)
{
    int c;
    FILE *arquivo_recordes = fopen("recordes.rcd", "r");

    if (arquivo_recordes == NULL)
        return;

    while ((c = fgetc(arquivo_recordes)) == '#')
        while ((c = fgetc(arquivo_recordes)) != '\n');
    ungetc(c, arquivo_recordes);

    fread(&collector->contexto.recordes, sizeof(collector->contexto.recordes), 1,
        arquivo_recordes);
}

static void salva_recordes(struct collector *collector)
{
    FILE *arquivo_recordes = fopen("recordes.rcd", "w");

    if (arquivo_recordes == NULL)
        return;

    fprintf(arquivo_recordes,
        "# Aposto que entrou aqui por curiosidade não é? ;) mas lembresse:\n"
        "#  'A curiosodade matou o gato'\n"
        "# então aconselho que não toque nas próximas linhas nem mesmo em um byte!\n");

    fwrite(&collector->contexto.recordes, sizeof(collector->contexto.recordes),
        1, arquivo_recordes);
}

static void animacao_comum(struct collector *collector)
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
            textbackground(collector->contexto.cores_fundo_animacao_normal[i - 2]);

            gotoxy(1, i - 1);
            cprintf("%s", linha_vazia);

            textbackground(collector->contexto.cores_fundo_animacao_normal[LINHAS - (i - 2) - 1]);
            gotoxy(1, LINHAS - (i - 2));
            cprintf("%s", linha_vazia);
        }

        gotoxy(METADE_COLUNAS, METADE_LINHAS);
        msleep(50);
    }

    textbackground(BLACK);
    for (j = METADE_COLUNAS; j > 0; --j) {
        for (i = 0; i < 2; ++i) {
            textbackground(collector->contexto.cores_fundo_animacao_normal[METADE_LINHAS + i]);
            gotoxy(j, METADE_LINHAS + i);
            cprintf(" ");

            gotoxy(COLUNAS - (j - 1), METADE_LINHAS + i);
            cprintf(" ");
        }

        gotoxy(METADE_COLUNAS, LINHAS);
        msleep(20);
    }

    memset(collector->contexto.cores_fundo_animacao_normal, BLACK,
        sizeof(collector->contexto.cores_fundo_animacao_normal));
}

static void animacao_gameover(struct collector *collector)
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

    (void) collector;
}

static ma_uint32 collector_audio_mixer(ma_decoder *audio, float *saida,
    ma_uint64 frames)
{
    ma_result result;

    ma_uint64 i;
    ma_uint64 frames_lidos = 0;
    ma_uint64 frames_para_ler = 0;
    ma_uint64 frames_totais_lidos = 0;

    float frames_em_buffer[2048];

    while (frames_totais_lidos < frames) {
        frames_para_ler = ma_countof(frames_em_buffer) / COLLECTOR_CANAIS_AUDIO;
        if (frames_para_ler > frames - frames_totais_lidos)
            frames_para_ler = frames - frames_totais_lidos;

        result = ma_data_source_read_pcm_frames(audio, frames_em_buffer,
            frames_para_ler, &frames_lidos);
        if (result != MA_SUCCESS || frames_para_ler == 0
            || frames_lidos < frames_para_ler)
            break;

        for (i = 0; i < frames_lidos * COLLECTOR_CANAIS_AUDIO; ++i)
            saida[frames_totais_lidos * COLLECTOR_CANAIS_AUDIO + i] +=
                frames_em_buffer[i];

        frames_totais_lidos += frames_lidos;
    }

    return frames_totais_lidos;
}

static void collector_audio(ma_device *dispositivo, void *saida,
    const void *entrada, ma_uint32 frames)
{
    int i;
    struct collector_audio *audio = dispositivo->pUserData;


    collector_audio_mixer(&audio->fundo, saida, frames);
    for (i = 0; i < COLLECTOR_AUDIOS; ++i) {
        if (audio->lista[i].finalizado)
            continue;

        if (collector_audio_mixer(&audio->lista[i].audio, saida, frames) < frames)
            audio->lista[i].finalizado = 1;
    }

    ma_apply_volume_factor_pcm_frames_f32(saida, frames,
        COLLECTOR_CANAIS_AUDIO, 0.4f);

    (void) entrada;
}

static void inicializa_audio(struct collector *collector)
{
    int i;
    char audios[COLLECTOR_AUDIOS][100] = {
        "assets/efeito.mp3"
    };

    ma_result result;

    ma_decoder_config config_audio =
        ma_decoder_config_init(COLLECTOR_FORMATO_AUDIO, COLLECTOR_CANAIS_AUDIO,
            COLLECTOR_FREQUENCIA_AUDIO);

    ma_device_config config_dispositivo_audio =
        ma_device_config_init(ma_device_type_playback);

    /* Carrega a música de fundo */
    result = ma_decoder_init_file("assets/musica.mp3", &config_audio,
        &collector->audio.fundo);
    if (result != MA_SUCCESS)
        return;

    ma_data_source_set_looping(&collector->audio.fundo, MA_TRUE);

    /* Carrega o restante dos audios */
    for (i = 0; i < COLLECTOR_AUDIOS; ++i) {
        collector->audio.lista[i].finalizado = 1;

        result = ma_decoder_init_file(audios[i], &config_audio,
            &collector->audio.lista[i].audio);
        if (result != MA_SUCCESS)
            return;
    }

    /* Cria o dispositivo de playback de audio */
    config_dispositivo_audio.playback.channels = COLLECTOR_CANAIS_AUDIO;
    config_dispositivo_audio.playback.format = COLLECTOR_FORMATO_AUDIO;
    config_dispositivo_audio.sampleRate = COLLECTOR_FREQUENCIA_AUDIO;
    config_dispositivo_audio.dataCallback = collector_audio;
    config_dispositivo_audio.pUserData = &collector->audio;

    result = ma_device_init(NULL, &config_dispositivo_audio,
        &collector->audio.dispositivo);
    if (result != MA_SUCCESS)
        return;

    ma_device_start(&collector->audio.dispositivo);
}

