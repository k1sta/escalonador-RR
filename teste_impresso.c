#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "universal.h"

#define ARQ_INPUT "input.txt"

void imprimirTabelaProcessos(Processo* processos, int numProcessos) {
    printf("\n\n\n");
    printf(" _______________________________________________________________________________\n");
    printf("|PID\t|Tempo de Entrada\t|Tempo de Execucao\t|Quantidade de IOs\t|\n");
    printf("--------------------------------------------------------------------------------|\n");
    for (int i = 0; i < numProcessos; i++) {
        printf("|%d\t|%d\t\t\t|%d\t\t\t|%d\t\t\t|\n", processos[i].PID, processos[i].tempoEntrada, processos[i].tempoExec, processos[i].qntdIO);
    }
    printf(" -------------------------------------------------------------------------------\n");
    printf("\n\n\n");
}

int main(void){
    Processo* listaProcessos = NULL;
    int numProcessos = contarLinhas(ARQ_INPUT);

    lerProcessos(ARQ_INPUT, &(listaProcessos), numProcessos);
     
    imprimirTabelaProcessos(listaProcessos, numProcessos);

    // Free allocated memory
    for (int i = 0; i < numProcessos; i++) {
        free(listaProcessos[i].ios);
    }
    free(listaProcessos);

    return 0;
}