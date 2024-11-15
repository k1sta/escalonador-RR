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


    int t = 0;
    Processo* processoEmExecucao = NULL;
    while(processosConcluidos != numProcessos){
	    // 1. verificar se chegam processos novos (Alta prioridade)
	    for (int i = 0; i < numProcessos; i++){
		    if (listaProcessos[i].tempoEntrada == t){
			    inserirFila(filaAltaP, listaProcessos[i]);
		    }
	    }

	    // 2. verificar se retornam processos das filas de IO (Prioridade relativa ao tipo de IO)
	    if (filaDiscoIO->inicio != NULL){
		    if(filaDiscoIO->inicio->ios[filaDiscoIO->inicio->proxIO]->tempoExecRestante == 0) {
			    filaDiscoIO->inicio->proxIO++;
			    inserirFila(filaBaixaP, removerFila(filaDiscoIO));
		    }
		    else filaDiscoIO->inicio->ios[filaDiscoIO->inicio->proxIO]->tempoExecRestante--;
	    } 

	    if (filaImpressoraIO->inicio != NULL){
		    if(filaImpressoraIO->inicio->ios[filaImpressoraIO->inicio->proxIO]->tempoExecRestante == 0) {
			    filaImpressoraIO->inicio->proxIO++;
			    inserirFila(filaAltaP, removerFila(filaImpressoraIO));
		    }
		    else filaImpressoraIO->inicio->ios[filaImpressoraIO->inicio->proxIO]->tempoExecRestante == 0;
	    }

	    if (filaFitaIO->inicio != NULL){
		    if(filaFitaIO->inicio->ios[filaFitaIO->inicio->proxIO]->tempoExecRestante == 0) {
			    filaFitaIO->inicio->proxIO++;
			    inserirFila(filaAltaP, removerFila(filaFitaIO));
		    }
		    else filaImpressoraIO->inicio->tempoExecRestante == 0;
	    }
	    
	    // 3. verificar se esse processo sofre IO no instante t de execução
	    // 4. verificar se esse processo sofre preempção no instante t (se sim, entra em Baixa Prioridade)
	    
	    if (processoEmExecucao != NULL){
		    if (processoEmExecucao->ios[processoEmExecucao->proxIO]->inicio == )
	    }

	    t++;
    }


    // Removendo filas e listas
    deletarFila(filaAltaP); deletarFila(filaBaixaP); deletarFila(filaDiscoIO); deletarFila(filaFitaIO); deletarFila(filaImpressoraIO);
    for (int i = 0; i < numProcessos; i++) {
        free(listaProcessos[i].ios);
    }
    free(listaProcessos);
}


