#include <stdio.h>
#include <stdbool.h>
#include "universal.h"

//Consideramos o valor da fatia como 3 iterações do loop.

#define ARQ_INPUT "input.txt"

int main(void){
    // Declarando filas
    FILA* filaAltaP = (FILA*)malloc(sizeof(FILA));
    FILA* filaBaixaP = (FILA*)malloc(sizeof(FILA));
    FILA* filaDiscoIO = (FILA*)malloc(sizeof(FILA));
    FILA* filaFitaIO = (FILA*)malloc(sizeof(FILA));
    FILA* filaImpressoraIO = (FILA*)malloc(sizeof(FILA));
    Processo* listaProcessos = NULL;
    int numProcessos, processosConcluidos = 0;

    if (filaAltaP == NULL || filaBaixaP == NULL || filaDiscoIO == NULL || filaFitaIO == NULL || filaImpressoraIO == NULL) {
        printf("Erro ao alocar memória para as filas.\n");
        return -1;
    }

    // Inicializando filas
    initFila(filaAltaP); initFila(filaBaixaP); initFila(filaDiscoIO); initFila(filaFitaIO); initFila(filaImpressoraIO);

    // Inicializando lista absoluta de processos e pegando o número de procesos
    numProcessos = contarLinhas(ARQ_INPUT);
    lerProcessos(ARQ_INPUT, &(listaProcessos), numProcessos);


    int t = 0
    while(processosConcluidos != numProcessos){
	    // 1. verificar se chegam processos novos (Alta prioridade)
	    // 2. verificar se retornam processos das filas de IO (Prioridade relativa ao tipo de IO)
	    // 3. verificar se esse processo sofre IO no instante t de execução
	    // 4. verificar se esse processo sofre preempção no instante t
	    t++;
    }


    // Removendo filas e listas
    deletarFila(filaAltaP); deletarFila(filaBaixaP); deletarFila(filaDiscoIO); deletarFila(filaFitaIO); deletarFila(filaImpressoraIO);
    for (int i = 0; i < numProcessos; i++) {
        free(listaProcessos[i].ios);
    }
    free(listaProcessos);
}


