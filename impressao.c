#include <stdio.h>
#include "universal.h"

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
    printf("______________________________________________________________________\n");
    printf("|PID\t|Qntd de IO\t|Tipo de IO\t|Início\t|Tempo Rest\t|\n");
    printf("|---------------------------------------------------------------------|\n");
    for (int i = 0; i < numProcessos; i++) {
        printf("|%d\t|%d\t\t", processos[i].PID, processos[i].qntdIO);
    	if(processos[i].qntdIO == 0){
		printf("%s", "|\t\t|\t|\t\t||\t\t|\n");
		continue;
	}
	for (int j = 0; j < processos[i].qntdIO; j++){
		printf("|%c\t\t|%d\t|%d\t\t|\n", processos[i].ios[j].tipo, processos[i].ios[j].inicio, processos[i].ios[j].tempoExecRestante);
		if(j != processos[i].qntdIO - 1){
			printf("%s", "|\t|\t\t");
		}
	}
    }

    printf("|---------------------------------------------------------------------|\n");
    printf("\n\n\n");
}
