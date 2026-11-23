#ifndef ENTREGA_H
#define ENTREGA_H

typedef struct {
    int a;
    int b;
} Restricao;

typedef struct {
    int n;
    int m;
    long long c;
    long long *pesos; /* indexado de 1 a n */
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

/* Opcoes da busca: ambas ficam ligadas por padrao. */
extern int corteViabilidade;
extern int corteOtimalidade;
extern long long nosExplorados;

/* Le uma instancia de stdin. Retorna 1 em caso de sucesso e 0 em erro. */
int lerEntrada(Instancia *instancia);

/* Libera os vetores alocados por lerEntrada. */
void liberaInstancia(Instancia *instancia);

/* Bound do professor: maximo entre |P| e ceil(sum(pesos) / C). */
long long limitanteDada(const Instancia *instancia, int viagensParciais);

/* Imprime a solucao no formato exigido pelo enunciado. */
void imprimeSaida(int k, const int *viagem, int n);

/* Resolve a instancia e imprime a solucao encontrada em stdout. */
int resolveProblema(const Instancia *instancia);

int constroiOrdem(const Instancia *instancia, int *ordem);
int constroiSolucaoInicial(Busca *busca);
void ramifica(Busca *busca, int pos, int k);

#endif
