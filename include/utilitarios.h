#ifndef COLLECTOR_UTILITARIOS_H
#define COLLECTOR_UTILITARIOS_H

#define COLUNAS        80
#define LINHAS         24

#define METADE_COLUNAS (COLUNAS / 2)
#define METADE_LINHAS  (LINHAS / 2)

/* msleep
 *
 * Função para pausar a execução do programa em alguns milisegundos.
 */
void msleep(int msec);

/* desenha_moldura
 */
void desenha_moldura(int transparente, int x, int y, int largura, int altura);

/* desenha_nome_jogo
 */
void desenha_nome_jogo(int x, int y);

/* desenha_linha_recorde
 */
void desenha_linha_recorde(int x, int y, char *nome, int pontos, int posicao);

#endif /* COLLECTOR_UTILITARIOS_H */

