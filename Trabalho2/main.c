/*
 * main.c - Ponto de entrada do programa envio
 * CI1238 - Otimizacao / INFO7070 - Topicos Especiais I
 *
 * Uso:
 *   ./envio [-f] [-o] [-a] < entrada > saida
 *
 *   -f  desliga os cortes por viabilidade (capacidade / precedencia)
 *   -o  desliga os cortes por otimalidade (poda por limitante)
 *   -a  usa a funcao limitante dada pelo professor
 *       (default: usa a funcao limitante proposta pelos autores)
 *
 * Entrada/saida pelo stdin/stdout, conforme especificacao do trabalho.
 * Relatorio (numero de nos e tempo gasto) e impresso em stderr.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "envio.h"
#define _POSIX_C_SOURCE 200809L

int main(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "foa")) != -1) {
        switch (opt) {
            case 'f': cutFeas = 0; break;
            case 'o': cutOpt = 0; break;
            case 'a': useProfBound = 1; break;
            default:
                fprintf(stderr, "uso: %s [-f] [-o] [-a] < entrada > saida\n", argv[0]);
                exit(1);
        }
    }

    readInput();

    for (int i = 1; i <= n; i++) {
        if (w[i] > C) {
            fprintf(stderr, "item %d tem peso maior que a capacidade do caminhao\n", i);
            exit(1);
        }
        totalWeightSum += w[i];
    }
    profWeightBound = ceilDiv(totalWeightSum, C);

    buildTopoOrder();

    assignTrip  = calloc((size_t)n + 1, sizeof(int));
    bestAssign  = calloc((size_t)n + 1, sizeof(int));
    remainCap   = calloc((size_t)n + 1, sizeof(long long));
    chainLenArr = calloc((size_t)n + 1, sizeof(long long));

    greedyInitial();                 /* preenche bestK / bestAssign */
    memset(assignTrip, 0, ((size_t)n + 1) * sizeof(int));

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    branch(0, 0);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    fprintf(stderr, "nos explorados: %lld\n", nodeCount);
    fprintf(stderr, "tempo gasto (s): %.6f\n", elapsed);
    fprintf(stderr, "cortes viabilidade: %s | cortes otimalidade: %s | bound: %s\n",
            cutFeas ? "ligados" : "desligados",
            cutOpt ? "ligados" : "desligados",
            useProfBound ? "professor" : "autores");

    printf("%d\n", bestK);
    for (int i = 1; i <= n; i++) printf("%d\n", bestAssign[i]);

    return 0;
}
