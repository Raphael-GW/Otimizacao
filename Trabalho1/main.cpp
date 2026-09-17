#include <iostream>
using namespace std ;


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
struct Cabo{
    // indice da central, capacidade, custo, vazao de energia
    long long t, w, c, v;
};

int main () {

    long long h, l, r;
    int i, j ;
    HidroEletrica *Hidros;
    Central *Centrais;

    Hidros = (HidroEletrica *)malloc( sizeof(HidroEletrica) * h);
    Centrais = (Central *)malloc( sizeof(Central) * l);

    /*Leitura de Parametros*/
    for (i = 0; i < h; i++) {
        cin >> Hidros[i].m >> Hidros[i].f >> Hidros[i].c ;
    }
    for (i = 0; i < l; i++) {
        cin >> Centrais[i].d ;
    }
    for (i = 0; i < h; i++) {        

        cin >> Hidros[i].n ;
        Hidros[i].a = (Cabo *)malloc( sizeof(Cabo) * n); 
        for (j = 0; j < n; j++) {
            cin >> Hidros[i].a[j].t >> Hidros[i].a[j].w >> Hidros[i].a[j].c ;  
        }
    }
    for (i = 0; i < l; i++) {

        cin >> Centrais[i].n ;
        Centrais[i].a = (Cabo *)malloc( sizeof(Cabo) * n); 
        for (j = 0; j < n; j++) {
            cin >> Centrais[i].a[j].t >> Centrais[i].a[j].w >> Centrais[i].a[j].c ;  
        }
    }

    /*Saida*/
    cout << "min : " ;
    /*custo das hidreletricas*/
    for(i = 0; i < h; i++) {
        cout << Hidros[i].c << "x" << i +1 << " + ";
    }

    /*custo de cada cabo das hidreletricas*/
    for(i = 0; i < h; i++) {
        for(j = 0; j < Hidros[i].n; j++) {
            cout << Hidros[i].a[j].c << "y" << i +1 << " + ";
        }
    }

    /*custo de cada cabo das centrais*/
    for(i = 0; i < l; i++) {
        for(j = 0; j < Centrais[i].n -1; j++) {
            cout << Centrais[i].a[j].c << "y" << i +1 +h << " + ";
        }
        cout << Centrais[i].a[j].c << "y" << i +1 +h ;
    }




    

}
