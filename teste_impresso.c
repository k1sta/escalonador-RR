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

void imprimirTabelaIO(Processo* processos, int numProcessos) {
    printf("\n\n\n");
    printf(" _______________________________________________________________\n");
    printf("|PID\t|Qntd de IO\t|Tipo de IO\t|Início\t|Tempo de IO\t|\n");
    printf("----------------------------------------------------------------|\n");
    for (int i = 0; i < numProcessos; i++) {
        printf("|%d\t|%d\t\t", processos[i].PID, processos[i].qntdIO);
    	if(processos[i].qntdIO == 0){
		printf("%s", "|\t\t|\t|\t\t|\n");
		continue;
	}
	for (int j = 0; j < processos[i].qntdIO; j++){
		printf("|%c\t\t|%d\t|%d\t\t|\n", processos[i].ios[j].tipo, processos[i].ios[j].inicio, processos[i].ios[j].tempoExec);
		if(j != processos[i].qntdIO - 1){
			printf("%s", "|\t|\t\t");
		}
	}    
    }
    printf("----------------------------------------------------------------|\n");
    printf("\n\n\n");
}

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
