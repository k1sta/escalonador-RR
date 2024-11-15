#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "universal.h"

//Consideramos o valor da fatia como 3 iterações do loop.

#define ARQ_INPUT "input copy.txt"
#define TIME_SLICE 4

int main(void){
    // Declarando filas
    FILA* filaAltaP = (FILA*)malloc(sizeof(FILA));
    FILA* filaBaixaP = (FILA*)malloc(sizeof(FILA));
    FILA* filaDiscoIO = (FILA*)malloc(sizeof(FILA));
    FILA* filaFitaIO = (FILA*)malloc(sizeof(FILA));
    FILA* filaImpressoraIO = (FILA*)malloc(sizeof(FILA));
    Processo* listaProcessos = NULL;
    int numProcessos;

    if (filaAltaP == NULL || filaBaixaP == NULL || filaDiscoIO == NULL || filaFitaIO == NULL || filaImpressoraIO == NULL) {
        printf("Erro ao alocar memória para as filas.\n");
        return -1;
    }

    // Inicializando filas
    initFila(filaAltaP); initFila(filaBaixaP); initFila(filaDiscoIO); initFila(filaFitaIO); initFila(filaImpressoraIO);

    // Inicializando lista absoluta de processos e pegando o número de procesos
    numProcessos = contarLinhas(ARQ_INPUT);
    if (numProcessos < 0) {
		printf("Erro ao contar linhas do arquivo.\n");
		return -1;
	}
	if (lerProcessos(ARQ_INPUT, &(listaProcessos), numProcessos) < 0) {
		printf("Erro ao ler processos do arquivo.\n");
		return -1;
	}


	// variáveis auxiliares para o while
    int t = 0, processosConcluidos = 0, inicioQuantum = 0;
    Processo* processoEmExecucao = NULL;
    while(processosConcluidos < numProcessos){
		printf("Segundo %d:\n", t);
	    // 1. verificar se chegam processos novos (Alta prioridade)
	    for (int i = 0; i < numProcessos; i++){
		    if (listaProcessos[i].tempoEntrada == t){
			    inserirFila(filaAltaP, &listaProcessos[i]);
				printf("Processo %d entrou na lista de Alta Prioridade\n", i+1);
		    }
			// if (t < listaProcessos[i].tempoEntrada) break;
	    }

	    // 2. verificar se retornam processos das filas de IO (Prioridade relativa ao tipo de IO)
	    if (filaDiscoIO->inicio != NULL){
		    if(filaDiscoIO->inicio->chave->ios[filaDiscoIO->inicio->chave->proxIO].tempoExecRestante == 0) {
			    filaDiscoIO->inicio->chave->proxIO++;
			    inserirFila(filaBaixaP, removerFila(filaDiscoIO));
				printf("Processo %d saiu do IO tipo DISCO, entrou na fila de Baixa Prioridade\n", filaBaixaP->fim->chave->PID);
		    }
		    if(filaDiscoIO->inicio != NULL)
				filaDiscoIO->inicio->chave->ios[filaDiscoIO->inicio->chave->proxIO].tempoExecRestante--;
	    } 

	    if (filaImpressoraIO->inicio != NULL){
		    if(filaImpressoraIO->inicio->chave->ios[filaImpressoraIO->inicio->chave->proxIO].tempoExecRestante == 0) {
			    filaImpressoraIO->inicio->chave->proxIO++;
			    inserirFila(filaAltaP, removerFila(filaImpressoraIO));
				printf("Processo %d saiu do IO tipo IMPRESSORA, entrou na fila de Alta Prioridade\n", filaAltaP->fim->chave->PID);
		    }
		    if(filaImpressoraIO->inicio != NULL)
				filaImpressoraIO->inicio->chave->ios[filaImpressoraIO->inicio->chave->proxIO].tempoExecRestante--;
	    }

	    if (filaFitaIO->inicio != NULL){
		    if(filaFitaIO->inicio->chave->ios[filaFitaIO->inicio->chave->proxIO].tempoExecRestante == 0) {
			    filaFitaIO->inicio->chave->proxIO++;
			    inserirFila(filaAltaP, removerFila(filaFitaIO));
				printf("Processo %d saiu do IO tipo FITA, entrou na fila de Alta Prioridade\n", filaAltaP->fim->chave->PID);
		    }
		    if(filaFitaIO->inicio != NULL)
				filaFitaIO->inicio->chave->ios[filaFitaIO->inicio->chave->proxIO].tempoExecRestante--;
	    }
	    
	    
	    // 3. verificar se há processo em execução
	    if (processoEmExecucao == NULL){
			processoEmExecucao = removerFila(filaAltaP);
			if (processoEmExecucao != NULL) printf("Processo %d em execucao, vindo da fila alta.\n", processoEmExecucao->PID);

			if(processoEmExecucao == NULL) {
				processoEmExecucao = removerFila(filaBaixaP);
				if (processoEmExecucao != NULL) printf("Processo %d em execucao, vindo da fila baixa.\n", processoEmExecucao->PID);
			}
			
			inicioQuantum = t;
			printf("Quantum iniciou em %d\n", inicioQuantum);
	    }

		if (processoEmExecucao != NULL){

			// 5. verificar se esse processo acabou a execução
			if (processoEmExecucao->tempoExecRestante == 0){
				printf("Processo %d terminou sua execucao.\n", processoEmExecucao->PID);
				processosConcluidos++;
				processoEmExecucao = NULL;
			} else{
				processoEmExecucao->tempoExecRestante--;
				printf("Processo %d possui %d segundos restantes de execucao.\n", processoEmExecucao->PID, processoEmExecucao->tempoExecRestante);
			}
	

	    	// 4. verificar se esse processo sofre IO no instante n da execução do processo
			if(processoEmExecucao != NULL){
				if(processoEmExecucao->qntdIO > processoEmExecucao->proxIO){
					printf("Processo %d possui %d io's restantes (momento %d do processo)\n", processoEmExecucao->PID, processoEmExecucao->qntdIO - processoEmExecucao->proxIO, processoEmExecucao->tempoExec - processoEmExecucao->tempoExecRestante);
					if(processoEmExecucao->ios[processoEmExecucao->proxIO].inicio == processoEmExecucao->tempoExec - processoEmExecucao->tempoExecRestante){
						switch(processoEmExecucao->ios[processoEmExecucao->proxIO].tipo){
							case 'F':
								inserirFila(filaFitaIO, processoEmExecucao);
								printf("Inserindo processo %d na fila de IO da fita\n", processoEmExecucao->PID);
								break;
							case 'I':
								inserirFila(filaImpressoraIO, processoEmExecucao);
								printf("Inserindo processo %d na fila de IO da Imporessora\n", processoEmExecucao->PID);
								break;
							case 'D':
								inserirFila(filaDiscoIO, processoEmExecucao);
								printf("Inserindo processo %d na fila de IO da Disco\n", processoEmExecucao->PID);
								break;
						}
						processoEmExecucao = NULL;
					}
				}	
			}


			// 6. verificar se esse processo sofre preempção no instante t (se sim, entra em Baixa Prioridade)
			if (processoEmExecucao != NULL && t - inicioQuantum + 1 >= TIME_SLICE){
				printf("Processo %d sofreu preempcao (quantum iniciado em %d)\n", processoEmExecucao->PID, inicioQuantum);
				inserirFila(filaBaixaP, processoEmExecucao);
				processoEmExecucao = NULL;
			}
		}

		printf("Fila de Alta Prioridade: %s\n", filaAltaP->inicio ? "TEM GENTE" : "VAZIA");
		printf("Fila de Baixa Prioridade: %s\n", filaBaixaP->inicio ? "TEM GENTE" : "VAZIA");
		printf("Fila de IO do Disco: %s\n", filaDiscoIO->inicio ? "TEM GENTE" : "VAZIA");
		printf("Fila de IO da Fita: %s\n", filaFitaIO->inicio ? "TEM GENTE" : "VAZIA");
		printf("Fila de IO da Impressora %s\n", filaImpressoraIO->inicio ? "TEM GENTE" : "VAZIA");
		printf("Processo em Execucao: %s\n", processoEmExecucao ? "EXECUTING" : "NONE");

	    t++;
		printf("%s", "|-------------------------------------------------------------------------------\n");
    }

	printf("ACABOU BOA SORTE");
    // Removendo filas e listas
    deletarFila(filaAltaP); deletarFila(filaBaixaP); deletarFila(filaDiscoIO); deletarFila(filaFitaIO); deletarFila(filaImpressoraIO);
    for (int i = 0; i < numProcessos; i++) {
        free(listaProcessos[i].ios);
    }
    free(listaProcessos);
}


