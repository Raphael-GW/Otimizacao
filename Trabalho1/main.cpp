#include <iostream>
using namespace std ;


struct HidroEletrica {
    // capacidade, eficiencia, custo
    long long m, f, c;
    Cabo *a;
} ;
struct Central{
    //demanda
    long long d; 
    Cabo *a;
};
struct Cabo{
    // indice da central, capacidade, custo 
    long long t, w, c;
};

int main () {

    long long h, l, r;
    HidroEletrica *Hidros;
    Central *Centrais;

    Hidros = (HidroEletrica *)malloc( sizeof(HidroEletrica) * h);
    Centrais = (Central *)malloc( sizeof(Central) * l);

    /*Leitura de Parametros*/
    for (int i = 0; i < h; i++) {
        cin >> Hidros[i].m >> Hidros[i].f >> Hidros[i].c ;
    }
    for (int i = 0; i < l; i++) {
        cin >> Centrais[i].d ;
    }
    for (int i = 0; i < h; i++) {        
        int n;

        cin >> n ;
        Hidros[i].a = (Cabo *)malloc( sizeof(Cabo) * n); 
        for (int j = 0; j < n; j++) {
            cin >> Hidros[i].a[j].t >> Hidros[i].a[j].w >> Hidros[i].a[j].c ;  
        }
    }
    for (int i = 0; i < l; i++) {
        int n;

        cin >> n ;
        Centrais[i].a = (Cabo *)malloc( sizeof(Cabo) * n); 
        for (int j = 0; j < n; j++) {
            cin >> Centrais[i].a[j].t >> Centrais[i].a[j].w >> Centrais[i].a[j].c ;  
        }
    }

    

}
