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

void printEstadoDaFila(FILA* filaAltaP, FILA* filaBaixaP, FILA* filaDiscoIO, FILA* filaFitaIO, FILA* filaImpressoraIO, Processo* processoEmExecucao) {
    printf("\n\n");
    printf(" _______________________________________________________________________________\n");
    printf("|                              Estado das Filas                                 |\n");
    printf("|-------------------------------------------------------------------------------|\n");
    
    // Print High Priority Queue
    printf("| Fila de Alta Prioridade:\t");
    Elemento* current = filaAltaP->inicio;
    while (current != NULL) {
        printf("[P%d (Resta: %d)] ", current->chave->PID, current->chave->tempoExecRestante);
        current = current->prox;
    }
    if (filaAltaP->inicio == NULL) {
        printf("VAZIA");
    }
    printf("\n");

    // Print Low Priority Queue
    printf("| Fila de Baixa Prioridade:\t");
    current = filaBaixaP->inicio;
    while (current != NULL) {
        printf("[P%d (Resta: %d)] ", current->chave->PID, current->chave->tempoExecRestante);
        current = current->prox;
    }
    if (filaBaixaP->inicio == NULL) {
        printf("VAZIA");
    }
    printf("\n");

    // Print Disk IO Queue
    printf("| Fila de IO do Disco:\t\t");
    current = filaDiscoIO->inicio;
    while (current != NULL) {
        printf("[P%d (Resta: %d)] ", current->chave->PID, current->chave->ios[current->chave->proxIO].tempoExecRestante);
        current = current->prox;
    }
    if (filaDiscoIO->inicio == NULL) {
        printf("VAZIA");
    }
    printf("\n");

    // Print Tape IO Queue
    printf("| Fila de IO da Fita:\t\t");
    current = filaFitaIO->inicio;
    while (current != NULL) {
        printf("[P%d (Resta: %d)] ", current->chave->PID, current->chave->ios[current->chave->proxIO].tempoExecRestante);
        current = current->prox;
    }
    if (filaFitaIO->inicio == NULL) {
        printf("VAZIA");
    }
    printf("\n");

    // Print Printer IO Queue
    printf("| Fila de IO da Impressora:\t");
    current = filaImpressoraIO->inicio;
    while (current != NULL) {
        printf("[P%d (Resta: %d)] ", current->chave->PID, current->chave->ios[current->chave->proxIO].tempoExecRestante);
        current = current->prox;
    }
    if (filaImpressoraIO->inicio == NULL) {
        printf("VAZIA");
    }
    printf("\n");
    // Print Process in Execution
    printf("|-------------------------------------------------------------------------------|\n");
    printf("| Process in Execution: ");
    if (processoEmExecucao != NULL) {
        printf("[P%d (Resta: %d)]", processoEmExecucao->PID, processoEmExecucao->tempoExecRestante);
    } else {
        printf("NONE");
    }
    printf("\n");
    printf(" -------------------------------------------------------------------------------\n");
    printf("\n\n");

}
