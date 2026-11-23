#ifndef ENTREGA_H
#define ENTREGA_H

typedef struct {
    int a;
    int b;
} Restricao;

typedef struct {
    int n;
    int m;
    int c;
    int *pesos;       /* indexado de 1 a n */
    Restricao *restricoes;  /* indexado de 0 a m - 1 */
} Instancia;

typedef struct {
    const Instancia *instancia;
    int *ordem;
    int *viagem;
    int *melhorViagem;
    long long *capacidadeRestante;
    int melhorK;
} Busca;

/* Le uma instancia de stdin. Retorna 1 em caso de sucesso e 0 em erro. */
int lerEntrada(Instancia *instancia);

/* Libera os vetores alocados por lerEntrada. */
void liberaInstancia(Instancia *instancia);

#endif
