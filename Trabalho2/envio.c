/*
 * envio.c - Branch & Bound para o problema "Envio ordenado"
 * CI1238 - Otimizacao / INFO7070 - Topicos Especiais I
 *
 * Contem toda a logica (leitura, ordem topologica, heuristica gulosa,
 * funcoes limitantes e o Branch & Bound em si). A funcao main() fica
 * em main.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "envio.h"
#define _POSIX_C_SOURCE 200809L

/* ---------------------------------------------------------------- */
/* Definicao das variaveis globais (declaradas como extern em envio.h) */
/* ---------------------------------------------------------------- */
int n, m;
long long C;
long long *w;
int *predCount;
int **pred;
int *topoOrder;

int *assignTrip;
int *bestAssign;
long long *remainCap;
long long *chainLenArr;
int bestK;
long long nodeCount = 0;

int cutFeas = 1;
int cutOpt  = 1;
int useProfBound = 0;

long long totalWeightSum = 0;
long long profWeightBound;

/* ---------------------------------------------------------------- */
long long ceilDiv(long long a, long long b) {
    return (a + b - 1) / b;
}

/* ---------------------------------------------------------------- */
/* Leitura da entrada                                                */
/* ---------------------------------------------------------------- */
void readInput(void) {
    if (scanf("%d %d %lld", &n, &m, &C) != 3) {
        fprintf(stderr, "erro na leitura de n, m, C\n");
        exit(1);
    }

    w = calloc((size_t)n + 1, sizeof(long long));
    for (int i = 1; i <= n; i++) {
        if (scanf("%lld", &w[i]) != 1) {
            fprintf(stderr, "erro na leitura dos pesos\n");
            exit(1);
        }
    }

    int *pa = malloc((size_t)(m > 0 ? m : 1) * sizeof(int));
    int *pb = malloc((size_t)(m > 0 ? m : 1) * sizeof(int));
    predCount = calloc((size_t)n + 1, sizeof(int));

    for (int e = 0; e < m; e++) {
        int a, b;
        if (scanf("%d %d", &a, &b) != 2) {
            fprintf(stderr, "erro na leitura das restricoes de ordem\n");
            exit(1);
        }
        pa[e] = a; pb[e] = b;
        predCount[b]++;
    }

    pred = malloc(((size_t)n + 1) * sizeof(int *));
    for (int i = 1; i <= n; i++)
        pred[i] = malloc((size_t)(predCount[i] > 0 ? predCount[i] : 1) * sizeof(int));

    int *fillIdx = calloc((size_t)n + 1, sizeof(int));
    for (int e = 0; e < m; e++) {
        int b = pb[e];
        pred[b][fillIdx[b]++] = pa[e];
    }
    free(fillIdx);
    free(pa);
    free(pb);
}

/* ---------------------------------------------------------------- */
/* Ordem topologica (Kahn), desempatando por peso decrescente        */
/* ---------------------------------------------------------------- */
void buildTopoOrder(void) {
    int *inDeg = calloc((size_t)n + 1, sizeof(int));
    for (int i = 1; i <= n; i++) inDeg[i] = predCount[i];

    int *succCount = calloc((size_t)n + 1, sizeof(int));
    for (int i = 1; i <= n; i++)
        for (int k = 0; k < predCount[i]; k++)
            succCount[pred[i][k]]++;

    int **succ = malloc(((size_t)n + 1) * sizeof(int *));
    for (int i = 1; i <= n; i++)
        succ[i] = malloc((size_t)(succCount[i] > 0 ? succCount[i] : 1) * sizeof(int));

    int *fi = calloc((size_t)n + 1, sizeof(int));
    for (int i = 1; i <= n; i++)
        for (int k = 0; k < predCount[i]; k++) {
            int p = pred[i][k];
            succ[p][fi[p]++] = i;
        }
    free(fi);

    topoOrder = malloc((size_t)n * sizeof(int));
    char *done = calloc((size_t)n + 1, sizeof(char));
    int cnt = 0;

    while (cnt < n) {
        int best = -1;
        for (int i = 1; i <= n; i++) {
            if (!done[i] && inDeg[i] == 0) {
                if (best == -1 || w[i] > w[best]) best = i;
            }
        }
        if (best == -1) {
            fprintf(stderr, "erro: ciclo nas restricoes de precedencia\n");
            exit(1);
        }
        topoOrder[cnt++] = best;
        done[best] = 1;
        for (int k = 0; k < succCount[best]; k++) inDeg[succ[best][k]]--;
    }

    free(done);
    free(inDeg);
    for (int i = 1; i <= n; i++) free(succ[i]);
    free(succ);
    free(succCount);
}

/* ---------------------------------------------------------------- */
/* Heuristica gulosa (first-fit na ordem topologica) para obter uma  */
/* solucao inicial viavel, usada como limitante superior inicial.    */
/* ---------------------------------------------------------------- */
void greedyInitial(void) {
    long long *rem = calloc((size_t)n + 1, sizeof(long long));
    int *assign = calloc((size_t)n + 1, sizeof(int));
    int k = 0;

    for (int idx = 0; idx < n; idx++) {
        int item = topoOrder[idx];
        int minTrip = 0;
        for (int p = 0; p < predCount[item]; p++) {
            int tr = assign[pred[item][p]];
            if (tr > minTrip) minTrip = tr;
        }
        int placed = 0;
        for (int t = minTrip + 1; t <= k; t++) {
            if (rem[t] >= w[item]) {
                rem[t] -= w[item];
                assign[item] = t;
                placed = 1;
                break;
            }
        }
        if (!placed) {
            k++;
            rem[k] = C - w[item];
            assign[item] = k;
        }
    }

    bestK = k;
    memcpy(bestAssign, assign, ((size_t)n + 1) * sizeof(int));
    free(rem);
    free(assign);
}

/* ---------------------------------------------------------------- */
/* Funcao limitante dada pelo professor:                             */
/*   B_dada = max( |P|, (1/C) * soma de todos os pesos )              */
/* ---------------------------------------------------------------- */
long long boundProf(int k_open) {
    long long v = k_open;
    if (profWeightBound > v) v = profWeightBound;
    return v;
}

/* ---------------------------------------------------------------- */
/* Funcao limitante dos autores (ver explicacao detalhada no texto). */
/* ---------------------------------------------------------------- */
long long boundOurs(int idx, int k_open) {
    long long freeCap = 0;
    for (int t = 1; t <= k_open; t++) freeCap += remainCap[t];

    long long remWeight = 0;
    for (int pos = idx; pos < n; pos++) remWeight += w[topoOrder[pos]];

    long long extra = remWeight - freeCap;
    long long weightBound = k_open;
    if (extra > 0) weightBound = k_open + ceilDiv(extra, C);

    long long chainBound = k_open;
    for (int pos = idx; pos < n; pos++) {
        int item = topoOrder[pos];
        long long best = 1;
        int minTrip = 0;
        for (int p = 0; p < predCount[item]; p++) {
            int pr = pred[item][p];
            if (assignTrip[pr] == 0) {
                if (chainLenArr[pr] + 1 > best) best = chainLenArr[pr] + 1;
            } else if (assignTrip[pr] > minTrip) {
                minTrip = assignTrip[pr];
            }
        }
        chainLenArr[item] = best;
        long long cand = minTrip + best;
        if (cand > chainBound) chainBound = cand;
    }

    return weightBound > chainBound ? weightBound : chainBound;
}

/* ---------------------------------------------------------------- */
/* Verifica se a solucao completa corrente eh de fato viavel.        */
/* So eh necessaria quando os cortes por viabilidade estao           */
/* desligados (-f).                                                  */
/* ---------------------------------------------------------------- */
int checkValidFinal(int k_open) {
    for (int t = 1; t <= k_open; t++)
        if (remainCap[t] < 0) return 0;
    for (int i = 1; i <= n; i++)
        for (int p = 0; p < predCount[i]; p++)
            if (assignTrip[pred[i][p]] >= assignTrip[i]) return 0;
    return 1;
}

/* ---------------------------------------------------------------- */
/* Branch & Bound                                                    */
/* ---------------------------------------------------------------- */
void branch(int idx, int k_open) {
    nodeCount++;

    if (cutOpt) {
        long long bnd = useProfBound ? boundProf(k_open) : boundOurs(idx, k_open);
        if (bnd >= bestK) return; /* corte por otimalidade */
    }

    if (idx == n) {
        int valid = cutFeas ? 1 : checkValidFinal(k_open);
        if (valid && k_open < bestK) {
            bestK = k_open;
            memcpy(bestAssign, assignTrip, ((size_t)n + 1) * sizeof(int));
        }
        return;
    }

    int item = topoOrder[idx];
    int minTrip = 0;
    for (int p = 0; p < predCount[item]; p++) {
        int tr = assignTrip[pred[item][p]];
        if (tr > minTrip) minTrip = tr;
    }

    int startT = cutFeas ? (minTrip + 1) : 1;
    for (int t = startT; t <= k_open; t++) {
        if (cutFeas) {
            int precOk = (t > minTrip);
            int capOk = (remainCap[t] >= w[item]);
            if (!precOk || !capOk) continue;
        }

        long long before = remainCap[t];
        remainCap[t] -= w[item];
        assignTrip[item] = t;

        branch(idx + 1, k_open);

        remainCap[t] = before;
        assignTrip[item] = 0;
    }

    /* abrir uma nova viagem (sempre viavel, pois ja garantimos w[item] <= C) */
    k_open++;
    remainCap[k_open] = C - w[item];
    assignTrip[item] = k_open;

    branch(idx + 1, k_open);

    assignTrip[item] = 0;
    k_open--;
}
