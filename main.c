#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "universal.h"

//Consideramos o valor da fatia como 3 iterações do loop.

// TESTANDO COM O INPUT3.TXT, REPAREI QUE O CÓDIGO ESTÁ ERRADO! Estamos terminando a execução no segundo seguinte. Portanto, todos os processos estão rodando
// 1 segundo a mais do que deveriam. INVESTIGAR.

#define TIME_SLICE 5

void inicializarEstruturas(FILA** filaAltaP, FILA** filaBaixaP, FILA** filaDiscoIO, FILA** filaFitaIO, FILA** filaImpressoraIO, Processo** listaProcessos, int numProcessos);
void apagarEstruturas(FILA* filaAltaP, FILA* filaBaixaP, FILA* filaDiscoIO, FILA* filaFitaIO, FILA* filaImpressoraIO, Processo* listaProcessos, int numProcessos);


int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        printf("Uso: %s <input_file> [-s]\n", argv[0]);
        return -1;
    }
	if (argc == 3 && strcmp(argv[2], "-s") != 0) {
		printf("Uso: %s <input_file> [-s]\n", argv[0]);
		return -1;
	}

	// Coleta o nome do arquivo de input
    const char *inputFile = argv[1];

    // Declarando filas
    FILA* filaAltaP = NULL;
    FILA* filaBaixaP = NULL;
    FILA* filaDiscoIO = NULL;
    FILA* filaFitaIO = NULL;
    FILA* filaImpressoraIO = NULL;
	
	// Declarando variáveis relacionadas ao processo
    Processo* listaProcessos = NULL;
    int numProcessos;
	
	// Variáveis auxiliares para o While
    int t = 0, processosConcluidos = 0, inicioQuantum = 0;
    Processo* processoEmExecucao = NULL;
	
	
    // Pegando o número de procesos
    numProcessos = contarLinhas(inputFile);
	if (numProcessos < 0) {
		printf("Erro ao contar linhas do arquivo.\n");
		exit(-1);
	}
    
	// Inicializando estruturas
    inicializarEstruturas(&filaAltaP, &filaBaixaP, &filaDiscoIO, &filaFitaIO, &filaImpressoraIO, &listaProcessos, numProcessos);
	
	// Limpando a tela
	printf("\e[1;1H\e[2J");
	
	// Lendo os processos
	if (lerProcessos(inputFile, &(listaProcessos), numProcessos) != 0) {
		printf("Erro ao ler processos do arquivo.\n");
		exit(-4);
	}

	imprimirTabelaProcessos(listaProcessos, numProcessos);
	imprimirTabelaIO(listaProcessos, numProcessos);
	if (argc == 3 && strcmp(argv[2], "-s") == 0) {
		printf("Pressione ENTER para continuar...\n");
		getchar();
	}
	puts("\n\n");
	// Escalonador rodando
    while(processosConcluidos < numProcessos){
		printf("Segundo %d-%d:\n", t, t+1);
	    // 1. verificar se chegam processos novos (Alta prioridade)
	    for (int i = 0; i < numProcessos; i++){
		    if (listaProcessos[i].tempoEntrada == t){
			    inserirFila(filaAltaP, &listaProcessos[i]);
				printf("Processo %d entrou na lista de Alta Prioridade\n", listaProcessos[i].PID);
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

		// 3. verificar se esse processo sofre preempção no instante t (se sim, entra em Baixa Prioridade)
		if (processoEmExecucao != NULL && t - inicioQuantum >= TIME_SLICE){
			printf("Processo %d sofreu preempcao (quantum iniciado em %d)\n", processoEmExecucao->PID, inicioQuantum);
			inserirFila(filaBaixaP, processoEmExecucao);
			processoEmExecucao = NULL;
		}
	    
		// 4. verificar se há processo em execução
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
            
    		processoEmExecucao->tempoExecRestante--;

            // 5. verificar se esse processo acabou a execução
		    if (processoEmExecucao->tempoExecRestante == 0){
				printf("Processo %d possui %d segundos restantes de execucao.\n", processoEmExecucao->PID, processoEmExecucao->tempoExecRestante);
				printf("Processo %d terminou sua execucao.\n", processoEmExecucao->PID);
				processoEmExecucao->turnaround = (t+1) - processoEmExecucao->tempoEntrada;
				processosConcluidos++;
				processoEmExecucao = NULL;
		    } else{
				printf("Processo %d possui %d segundos restantes de execucao.\n", processoEmExecucao->PID, processoEmExecucao->tempoExecRestante);
		    }	
	

	    	// 6. verificar se esse processo sofre IO no instante n da execução do processo
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

		}
		
		printEstadoDaFila(filaAltaP, filaBaixaP, filaDiscoIO, filaFitaIO, filaImpressoraIO, processoEmExecucao);

	    t++;
		printf("%s", "--------------------------------------------------------------------------------\n");
		if (argc == 3 && strcmp(argv[2], "-s") == 0) getchar();
    }

    
	printf("Todos os processos foram concluidos.\nTempo de execução total: %d segundos\nTurnaround para cada processo:\n", t);
	for(int i = 0; i < numProcessos; i++){
		printf("\tProcesso %d: %d segundos\n", listaProcessos[i].PID, listaProcessos[i].turnaround);
	}

	// Removendo filas e listas
    apagarEstruturas(filaAltaP, filaBaixaP, filaDiscoIO, filaFitaIO, filaImpressoraIO, listaProcessos, numProcessos);
	return 0;
}

void inicializarEstruturas(FILA** filaAltaP, FILA** filaBaixaP, FILA** filaDiscoIO, FILA** filaFitaIO, FILA** filaImpressoraIO, Processo** listaProcessos, int numProcessos){
    if(initFila(filaAltaP) || initFila(filaBaixaP) || initFila(filaDiscoIO) || initFila(filaFitaIO) || initFila(filaImpressoraIO)){
        printf("Erro ao inicializar filas.\n");
        exit(-2);
    }
    
    *listaProcessos = (Processo*)malloc(numProcessos * sizeof(Processo));
    if (!*listaProcessos) {
        perror("Erro ao alocar memória para os processos.\n");
        exit(-3);
    }
}


void apagarEstruturas(FILA* filaAltaP, FILA* filaBaixaP, FILA* filaDiscoIO, FILA* filaFitaIO, FILA* filaImpressoraIO, Processo* listaProcessos, int numProcessos) {
    deletarFila(filaAltaP); 
    deletarFila(filaBaixaP);
    deletarFila(filaDiscoIO);
    deletarFila(filaFitaIO);
    deletarFila(filaImpressoraIO);
    for (int i = 0; i < numProcessos; i++) {
        free(listaProcessos[i].ios);
    }
    free(listaProcessos);
}