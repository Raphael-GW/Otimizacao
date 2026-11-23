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
    /* dados do limitante melhorado */
    int *cauda;        /* maior cadeia de precedencia comecando no item */
    int *inicio;       /* auxiliar: menor viagem possivel de cada item */
    int *predInicio;   /* predecessores de i: predLista[predInicio[i]..predInicio[i+1]-1] */
    int *predLista;
    int itensGrandes;  /* itens com peso > C/2: nunca dividem viagem */
    int limiteEstatico;
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

/* Limitante melhorado (>= limitanteDada). Exige preparaLimitante. Considera
 * precedencias, itens grandes e as viagens ja fixadas em busca->viagem. */
int preparaLimitante(Busca *busca);
void liberaLimitante(Busca *busca);
long long limitanteMelhor(Busca *busca, int pos, int k);

/* Imprime a solucao no formato exigido pelo enunciado. */
void imprimeSaida(int k, const int *viagem, int n);

/* Resolve a instancia e imprime a solucao encontrada em stdout. */
int resolveProblema(const Instancia *instancia);

int constroiOrdem(const Instancia *instancia, int *ordem);
int constroiSolucaoInicial(Busca *busca);
void ramifica(Busca *busca, int pos, int k);

#endif
