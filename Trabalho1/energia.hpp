#include <iostream>
#include <stdlib.h>
using namespace std ;

struct Cabo{
    // indice da central, capacidade, custo, vazao de energia
    long long t, w, c, v;
};
struct HidroEletrica {
    // capacidade, eficiencia, custo, vazao de agua, numero de cabos
    long long m, f, c, v, n;
    Cabo *a;
} ;
struct Central{
    //demanda, numero de cabos
    long long d, n; 
    Cabo *a;
};

void scan_input(long long h, long long l, HidroEletrica *&Hidros, Central *&Centrais) ;
void print_min_funcion(long long h, long long l, HidroEletrica *Hidros, Central *Centrais) ;
void print_restrictions(long long h, long long l, long long r, HidroEletrica *Hidros, Central *Centrais) ;
