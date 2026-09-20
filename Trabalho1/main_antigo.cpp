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

int main () {

    long long h, l, r;
    int i, j ;
    HidroEletrica *Hidros;
    Central *Centrais;

    cin >> h >> l >> r ;

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
        Hidros[i].a = (Cabo *)malloc( sizeof(Cabo) * Hidros[i].n); 
        for (j = 0; j < Hidros[i].n; j++) {
            cin >> Hidros[i].a[j].t >> Hidros[i].a[j].w >> Hidros[i].a[j].c ;  
        }
    }
    for (i = 0; i < l; i++) {

        cin >> Centrais[i].n ;
        Centrais[i].a = (Cabo *)malloc( sizeof(Cabo) * Centrais[i].n); 
        for (j = 0; j < Centrais[i].n; j++) {
            cin >> Centrais[i].a[j].t >> Centrais[i].a[j].w >> Centrais[i].a[j].c ;  
        }
    }

    /*Saida*/

    /*funcao minimizadora*/
    cout << "min : " ;
    /*custo das hidreletricas*/
    for(i = 0; i < h; i++) {
        if (i > 0) cout << " + ";
        cout << Hidros[i].c << "x" << i +1;
    } 

    long long k = 1;

    /* custo dos cabos das hidrelétricas */
    for (i = 0; i < h; i++) {
        for (j = 0; j < Hidros[i].n; j++) {
            cout << " + " << Hidros[i].a[j].c << " y" << k++;
        }
    }

    /* custo dos cabos das centrais */
    for (i = 0; i < l; i++) {
        for (j = 0; j < Centrais[i].n; j++) {
            cout << " + " << Centrais[i].a[j].c << " y" << k++;
        }
    }

    cout << ";\n";
        
    /* Vazao e capacidade de producao */
    for (i = 0; i < h; i++) {
        cout << "x" << i + 1 << " >= 0;\n";
        cout << "x" << i + 1 << " <= " << r << ";\n";

        cout << Hidros[i].f << " x" << i + 1
            << " <= " << Hidros[i].m << ";\n";
    }

    /* Energia produzida = soma da energia enviada */
    k = 1;

    for (i = 0; i < h; i++) {
        cout << Hidros[i].f << " x" << i + 1;

        for (j = 0; j < Hidros[i].n; j++) {
            cout << " - y" << k++;
        }

        cout << " = 0;\n";
    }

    /* Capacidades dos cabos, na ordem da funcao objetivo */
    k = 1;

    for (i = 0; i < h; i++) {
        for (j = 0; j < Hidros[i].n; j++) {
            cout << "y" << k << " >= 0;\n";
            cout << "y" << k << " <= "
                << Hidros[i].a[j].w << ";\n";
            k++;
        }
    }

    for (i = 0; i < l; i++) {
        for (j = 0; j < Centrais[i].n; j++) {
            cout << "y" << k << " >= 0;\n";
            cout << "y" << k << " <= "
                << Centrais[i].a[j].w << ";\n";
            k++;
        }
    }

    /* Atendimento das demandas: entradas - saidas >= demanda */
    for (int central = 0; central < l; central++) {
        k = 1;

        // Comecar com zero simplifica a impressao dos sinais.
        cout << "0";

        /* Entradas vindas das hidreletricas */
        for (i = 0; i < h; i++) {
            for (j = 0; j < Hidros[i].n; j++) {
                if (Hidros[i].a[j].t == central + 1) {
                    cout << " + y" << k;
                }
                k++;
            }
        }

        /* Entradas e saidas envolvendo centrais */
        for (i = 0; i < l; i++) {
            for (j = 0; j < Centrais[i].n; j++) {
                if (Centrais[i].a[j].t == central + 1) {
                    cout << " + y" << k;
                }

                if (i == central) {
                    cout << " - y" << k;
                }

                k++;
            }
        }

        cout << " >= " << Centrais[central].d << ";\n";
    }

}
