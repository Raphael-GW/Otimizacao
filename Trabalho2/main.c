#include "entrega.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void imprimeUso(const char *programa) {
    fprintf(stderr, "uso: %s [-f] [-o] < entrada\n", programa);
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            corteViabilidade = 0;
        } else if (strcmp(argv[i], "-o") == 0) {
            corteOtimalidade = 0;
        } else {
            imprimeUso(argv[0]);
            return EXIT_FAILURE;
        }
    }

    Instancia instancia;

    if (!lerEntrada(&instancia))
        return 1;


    nosExplorados = 0;

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    if (!resolveProblema(&instancia)) {
        liberaInstancia(&instancia);
        return 1;
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) +
                (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    fprintf(stderr, "tempo de execucao: %.9f segundos\n", tempo);

    
    fprintf(stderr, "nos explorados: %lld\n", nosExplorados);
    fprintf(stderr, "tempo de execucao: %ld segundos\n", (long)tempo);

    liberaInstancia(&instancia);
    return 0;
}
