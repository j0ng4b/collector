#ifndef COLLECTOR_UTILITARIOS_H
#define COLLECTOR_UTILITARIOS_H

#define COLUNAS                       80
#define LINHAS                        24

#define METADE_COLUNAS                (COLUNAS / 2)
#define METADE_LINHAS                 (LINHAS / 2)

/* msleep
 *
 * Função para pausar a execução do programa em alguns milisegundos.
 */
void msleep(int msec);

/* desenha_moldura
 */
void desenha_moldura(int transparente, int x, int y, int largura, int altura);

#endif /* COLLECTOR_UTILITARIOS_H */

