#include <iostream>
#include <stdlib.h>

#include "energia.hpp"

using namespace std ;

int main () {

    long long h, l, r;
    HidroEletrica *Hidros;
    Central *Centrais;

    cin >> h >> l >> r ;

    scan_input(h, l, Hidros, Centrais);

    /*Saida*/
    /*funcao minimizadora*/
    print_min_funcion(h, l, Hidros, Centrais);

    /*restricoes*/
    print_restrictions(h, l, r, Hidros, Centrais);

}
