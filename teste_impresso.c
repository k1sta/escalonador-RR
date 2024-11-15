#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "universal.h"

#define ARQ_INPUT "input.txt"

int main(void){
    Processo* listaProcessos = NULL;
    int numProcessos = contarLinhas(ARQ_INPUT);

    lerProcessos(ARQ_INPUT, &(listaProcessos), numProcessos);
     
    imprimirTabelaProcessos(listaProcessos, numProcessos);
    imprimirTabelaIO(listaProcessos, numProcessos);

    // Free allocated memory
    for (int i = 0; i < numProcessos; i++) {
        free(listaProcessos[i].ios);
    }
    free(listaProcessos);

    return 0;
}
