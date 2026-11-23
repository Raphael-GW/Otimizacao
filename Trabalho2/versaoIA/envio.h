/*
 * envio.h - Declaracoes do Branch & Bound para o problema "Envio ordenado"
 * CI1238 - Otimizacao / INFO7070 - Topicos Especiais I
 */

#ifndef ENVIO_H
#define ENVIO_H

#include <stddef.h>

/* ---------------------------------------------------------------- */
/* Dados da instancia                                                */
/* ---------------------------------------------------------------- */
extern int n, m;
extern long long C;
extern long long *w;          /* pesos, indexado 1..n                */
extern int *predCount;        /* qtd de predecessores diretos de i   */
extern int **pred;            /* pred[i][] = predecessores diretos   */
extern int *topoOrder;        /* ordem topologica (itens 1..n)       */

/* ---------------------------------------------------------------- */
/* Estado de busca (Branch & Bound)                                  */
/* ---------------------------------------------------------------- */
extern int *assignTrip;       /* viagem atribuida a cada item (0 = nao atribuido) */
extern int *bestAssign;       /* melhor solucao encontrada ate agora */
extern long long *remainCap;  /* capacidade restante de cada viagem aberta, 1..k */
extern long long *chainLenArr;/* auxiliar para o bound dos autores   */
extern int bestK;             /* valor da melhor solucao (numero de viagens) */
extern long long nodeCount;

/* flags de linha de comando */
extern int cutFeas;       /* cortes por viabilidade ativos?  (desliga com -f) */
extern int cutOpt;        /* cortes por otimalidade ativos?  (desliga com -o) */
extern int useProfBound;  /* usa a bound do professor?       (liga com -a)   */

extern long long totalWeightSum;
extern long long profWeightBound;   /* ceil(soma de todos os pesos / C) */

/* ---------------------------------------------------------------- */
/* Funcoes                                                           */
/* ---------------------------------------------------------------- */
long long ceilDiv(long long a, long long b);

void readInput(void);
void buildTopoOrder(void);
void greedyInitial(void);

long long boundProf(int k_open);
long long boundOurs(int idx, int k_open);

int checkValidFinal(int k_open);

void branch(int idx, int k_open);

#endif /* ENVIO_H */
