#include "entrega.h"

#include <stdio.h>
#include <stdlib.h>

int corteViabilidade = 1;
int corteOtimalidade = 1;
long long nosExplorados = 0;

int lerEntrada(Instancia *instancia) {
    if (instancia == NULL)
        return 0;

    instancia->n = 0;
    instancia->m = 0;
    instancia->c = 0;
    instancia->pesos = NULL;
    instancia->restricoes = NULL;

    if (scanf("%d %d %lld", &instancia->n, &instancia->m,
              &instancia->c) != 3) {
        fprintf(stderr, "erro: nao foi possivel ler n, m e C\n");
        return 0;
    }

    if (instancia->n < 0 || instancia->m < 0 || instancia->c <= 0) {
        fprintf(stderr, "erro: valores invalidos para n, m ou C\n");
        return 0;
    }

    instancia->pesos = calloc((size_t)instancia->n + 1,
                               sizeof(*instancia->pesos));
    if (instancia->pesos == NULL) {
        fprintf(stderr, "erro: memoria insuficiente para os pesos\n");
        return 0;
    }

    for (int i = 1; i <= instancia->n; i++) {
        if (scanf("%lld", &instancia->pesos[i]) != 1) {
            fprintf(stderr, "erro: nao foi possivel ler o peso do item %d\n", i);
            liberaInstancia(instancia);
            return 0;
        }
    }

    if (instancia->m > 0) {
        instancia->restricoes = malloc((size_t)instancia->m *
                                       sizeof(*instancia->restricoes));
        if (instancia->restricoes == NULL) {
            fprintf(stderr, "erro: memoria insuficiente para as restricoes\n");
            liberaInstancia(instancia);
            return 0;
        }
    }

    for (int i = 0; i < instancia->m; i++) {
        if (scanf("%d %d", &instancia->restricoes[i].a,
                  &instancia->restricoes[i].b) != 2) {
            fprintf(stderr, "erro: nao foi possivel ler a restricao %d\n", i + 1);
            liberaInstancia(instancia);
            return 0;
        }

        if (instancia->restricoes[i].a < 1 || instancia->restricoes[i].a > instancia->n ||
            instancia->restricoes[i].b < 1 || instancia->restricoes[i].b > instancia->n) {
            fprintf(stderr, "erro: item invalido na restricao %d\n", i + 1);
            liberaInstancia(instancia);
            return 0;
        }
    }

    return 1;
}

void liberaInstancia(Instancia *instancia) {
    if (instancia == NULL)
        return;

    free(instancia->pesos);
    free(instancia->restricoes);
    instancia->pesos = NULL;
    instancia->restricoes = NULL;
}

long long limitanteDada(const Instancia *instancia, int viagensParciais) {
    if (instancia == NULL || instancia->c <= 0)
        return 0;

    long long somaPesos = 0;
    for (int i = 1; i <= instancia->n; i++)
        somaPesos += instancia->pesos[i];

    long long limitePorPeso = (somaPesos + instancia->c - 1) / instancia->c;
    return viagensParciais > limitePorPeso ? viagensParciais : limitePorPeso;
}

void imprimeSaida(int k, const int *viagem, int n) {
    printf("%d\n", k);
    for (int i = 1; i <= n; i++)
        printf("%d\n", viagem[i]);
}

/* Produz uma ordem topologica dos itens. */
int constroiOrdem(const Instancia *instancia, int *ordem) {
    int *grauEntrada = calloc((size_t)instancia->n + 1, sizeof(*grauEntrada));
    int *usado = calloc((size_t)instancia->n + 1, sizeof(*usado));
    if (grauEntrada == NULL || usado == NULL) {
        free(grauEntrada);
        free(usado);
        return 0;
    }

    for (int i = 0; i < instancia->m; i++)
        grauEntrada[instancia->restricoes[i].b]++;

    for (int pos = 0; pos < instancia->n; pos++) {
        int item = 0;
        for (int i = 1; i <= instancia->n; i++) {
            if (!usado[i] && grauEntrada[i] == 0) {
                item = i;
                break;
            }
        }

        if (item == 0) {
            free(grauEntrada);
            free(usado);
            return 0;
        }

        ordem[pos] = item;
        usado[item] = 1;
        for (int i = 0; i < instancia->m; i++)
            if (instancia->restricoes[i].a == item)
                grauEntrada[instancia->restricoes[i].b]--;
    }

    free(grauEntrada);
    free(usado);
    return 1;
}

/* Gera uma solucao inicial viavel para fornecer um limite superior. */
int constroiSolucaoInicial(Busca *busca) {
    const Instancia *instancia = busca->instancia;
    long long *restante = calloc((size_t)instancia->n + 1, sizeof(*restante));
    int *solucao = calloc((size_t)instancia->n + 1, sizeof(*solucao));
    if (restante == NULL || solucao == NULL) {
        free(restante);
        free(solucao);
        return 0;
    }

    int k = 0;
    for (int pos = 0; pos < instancia->n; pos++) {
        int item = busca->ordem[pos];
        int menorViagem = 0;
        for (int i = 0; i < instancia->m; i++)
            if (instancia->restricoes[i].b == item &&
                solucao[instancia->restricoes[i].a] > menorViagem)
                menorViagem = solucao[instancia->restricoes[i].a];

        int colocado = 0;
        for (int viagem = menorViagem + 1; viagem <= k; viagem++) {
            if (restante[viagem] >= instancia->pesos[item]) {
                restante[viagem] -= instancia->pesos[item];
                solucao[item] = viagem;
                colocado = 1;
                break;
            }
        }

        if (!colocado) {
            k++;
            restante[k] = instancia->c - instancia->pesos[item];
            solucao[item] = k;
        }
    }

    busca->melhorK = k;
    for (int i = 1; i <= instancia->n; i++)
        busca->melhorViagem[i] = solucao[i];

    free(restante);
    free(solucao);
    return 1;
}

/* Verifica uma atribuicao completa quando os cortes de viabilidade estao
 * desligados. Nesse modo, a busca pode visitar estados com excesso de carga
 * ou precedencia violada, mas nunca pode aceitar uma folha inviavel. */
static int solucaoViavel(const Busca *busca, int k) {
    const Instancia *instancia = busca->instancia;
    long long *carga = calloc((size_t)k + 1, sizeof(*carga));
    if (carga == NULL)
        return 0;

    for (int item = 1; item <= instancia->n; item++) {
        int viagem = busca->viagem[item];
        if (viagem < 1 || viagem > k) {
            free(carga);
            return 0;
        }
        carga[viagem] += instancia->pesos[item];
        if (carga[viagem] > instancia->c) {
            free(carga);
            return 0;
        }
    }

    for (int i = 0; i < instancia->m; i++) {
        int origem = instancia->restricoes[i].a;
        int destino = instancia->restricoes[i].b;
        if (busca->viagem[origem] >= busca->viagem[destino]) {
            free(carga);
            return 0;
        }
    }

    free(carga);
    return 1;
}

void ramifica(Busca *busca, int pos, int k) {
    const Instancia *instancia = busca->instancia;
    nosExplorados++;

    if (corteOtimalidade &&
        limitanteDada(instancia, k) >= busca->melhorK)
        return;

    if (pos == instancia->n) {
        if ((!corteViabilidade && solucaoViavel(busca, k)) ||
            (corteViabilidade && k < busca->melhorK)) {
            if (k >= busca->melhorK)
                return;
            busca->melhorK = k;
            for (int i = 1; i <= instancia->n; i++)
                busca->melhorViagem[i] = busca->viagem[i];
        }
        return;
    }

    int item = busca->ordem[pos];
    int menorViagem = 0;
    for (int i = 0; i < instancia->m; i++)
        if (instancia->restricoes[i].b == item &&
            busca->viagem[instancia->restricoes[i].a] > menorViagem)
            menorViagem = busca->viagem[instancia->restricoes[i].a];

    int primeiraViagem = corteViabilidade ? menorViagem + 1 : 1;
    for (int viagem = primeiraViagem; viagem <= k; viagem++) {
        if (corteViabilidade &&
            busca->capacidadeRestante[viagem] < instancia->pesos[item])
            continue;

        busca->capacidadeRestante[viagem] -= instancia->pesos[item];
        busca->viagem[item] = viagem;
        ramifica(busca, pos + 1, k);
        busca->viagem[item] = 0;
        busca->capacidadeRestante[viagem] += instancia->pesos[item];
    }

    busca->capacidadeRestante[k + 1] = instancia->c - instancia->pesos[item];
    busca->viagem[item] = k + 1;
    ramifica(busca, pos + 1, k + 1);
    busca->viagem[item] = 0;
    busca->capacidadeRestante[k + 1] = 0;
}

int resolveProblema(const Instancia *instancia) {
    if (instancia == NULL)
        return 0;

    for (int i = 1; i <= instancia->n; i++) {
        if (instancia->pesos[i] < 0 || instancia->pesos[i] > instancia->c) {
            fprintf(stderr, "erro: peso invalido para o item %d\n", i);
            return 0;
        }
    }

    Busca busca = {0};
    busca.instancia = instancia;
    busca.ordem = malloc((size_t)instancia->n * sizeof(*busca.ordem));
    busca.viagem = calloc((size_t)instancia->n + 1, sizeof(*busca.viagem));
    busca.melhorViagem = calloc((size_t)instancia->n + 1,
                                sizeof(*busca.melhorViagem));
    busca.capacidadeRestante = calloc((size_t)instancia->n + 1,
                                      sizeof(*busca.capacidadeRestante));

    if ((instancia->n > 0 && busca.ordem == NULL) || busca.viagem == NULL ||
        busca.melhorViagem == NULL || busca.capacidadeRestante == NULL) {
        free(busca.ordem);
        free(busca.viagem);
        free(busca.melhorViagem);
        free(busca.capacidadeRestante);
        fprintf(stderr, "erro: memoria insuficiente para resolver a instancia\n");
        return 0;
    }

    if (!constroiOrdem(instancia, busca.ordem) ||
        !constroiSolucaoInicial(&busca)) {
        fprintf(stderr, "erro: as restricoes de precedencia formam um ciclo\n");
        free(busca.ordem);
        free(busca.viagem);
        free(busca.melhorViagem);
        free(busca.capacidadeRestante);
        return 0;
    }

    ramifica(&busca, 0, 0);
    imprimeSaida(busca.melhorK, busca.melhorViagem, instancia->n);

    free(busca.ordem);
    free(busca.viagem);
    free(busca.melhorViagem);
    free(busca.capacidadeRestante);
    return 1;
}
