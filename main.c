#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "universal.h"

#define TIME_SLICE 5

// Assinatura das funções para inicializar e remover estruturas de dados

void inicializarEstruturas(FILA** filaAltaP, FILA** filaBaixaP, FILA** filaDiscoIO, FILA** filaFitaIO, FILA** filaImpressoraIO, Processo** listaProcessos, int numProcessos);
void apagarEstruturas(FILA* filaAltaP, FILA* filaBaixaP, FILA* filaDiscoIO, FILA* filaFitaIO, FILA* filaImpressoraIO, Processo* listaProcessos, int numProcessos);


int main(int argc, char *argv[]) {

	// Se o programa for inicializado errado, gera erro
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

	// Tela mostrando os processos e seus I/O's, vindos do input do arquivo
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
	    }

	    // 2. verificar se retornam processos das filas de IO (Prioridade relativa ao tipo de IO)
		// Disco
	    if (filaDiscoIO->inicio != NULL){

			//se o tempo de execução do I/O for 0, retornamos o processo para a fila de baixa prioridade
		    if(filaDiscoIO->inicio->chave->ios[filaDiscoIO->inicio->chave->proxIO].tempoExecRestante == 0) {
			    filaDiscoIO->inicio->chave->proxIO++;
			    inserirFila(filaBaixaP, removerFila(filaDiscoIO));
				printf("Processo %d saiu do IO tipo DISCO, entrou na fila de Baixa Prioridade\n", filaBaixaP->fim->chave->PID);
		    }

			//se o I/O não tiver acabado, decrementamos 1 de seu tempo
		    if(filaDiscoIO->inicio != NULL)
				filaDiscoIO->inicio->chave->ios[filaDiscoIO->inicio->chave->proxIO].tempoExecRestante--;
	    } 

		// Fila
	    if (filaImpressoraIO->inicio != NULL){

			//se o tempo de execução do I/O for 0, retornamos o processo para a fila de alta prioridade
		    if(filaImpressoraIO->inicio->chave->ios[filaImpressoraIO->inicio->chave->proxIO].tempoExecRestante == 0) {
			    filaImpressoraIO->inicio->chave->proxIO++;
			    inserirFila(filaAltaP, removerFila(filaImpressoraIO));
				printf("Processo %d saiu do IO tipo IMPRESSORA, entrou na fila de Alta Prioridade\n", filaAltaP->fim->chave->PID);
		    }

			//se o I/O não tiver acabado, decrementamos 1 de seu tempo
		    if(filaImpressoraIO->inicio != NULL)
				filaImpressoraIO->inicio->chave->ios[filaImpressoraIO->inicio->chave->proxIO].tempoExecRestante--;
	    }

		// Fita
	    if (filaFitaIO->inicio != NULL){

			//se o tempo de execução do I/O for 0, retornamos o processo para a fila de alta prioridade
		    if(filaFitaIO->inicio->chave->ios[filaFitaIO->inicio->chave->proxIO].tempoExecRestante == 0) {
			    filaFitaIO->inicio->chave->proxIO++;
			    inserirFila(filaAltaP, removerFila(filaFitaIO));
				printf("Processo %d saiu do IO tipo FITA, entrou na fila de Alta Prioridade\n", filaAltaP->fim->chave->PID);
		    }

			//se o I/O não tiver acabado, decrementamos 1 de seu tempo
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

			//se não tiver processo em execução, pegamos um da fila de alta prioridade
			processoEmExecucao = removerFila(filaAltaP);
			if (processoEmExecucao != NULL) printf("Processo %d em execucao, vindo da fila alta.\n", processoEmExecucao->PID);

			//se não tiver processo na fila de alta prioridade, pegamos um da fila de baixa prioridade
			if(processoEmExecucao == NULL) {
				processoEmExecucao = removerFila(filaBaixaP);
				if (processoEmExecucao != NULL) printf("Processo %d em execucao, vindo da fila baixa.\n", processoEmExecucao->PID);
			}
			
			// inicioQuantum para monitorarmos o timeslice
			inicioQuantum = t;
			printf("Quantum iniciou em %d\n", inicioQuantum);
	    }

		//se tiver processo em execução, drecrementamos um de seu tempo restante
		if (processoEmExecucao != NULL){
            
    		processoEmExecucao->tempoExecRestante--;

            // 5. verificar se esse processo acabou a execução
		    if (processoEmExecucao->tempoExecRestante == 0){

				printf("Processo %d possui %d segundos restantes de execucao.\n", processoEmExecucao->PID, processoEmExecucao->tempoExecRestante);
				printf("Processo %d terminou sua execucao.\n", processoEmExecucao->PID);

				//calculando turnaround do processo
				processoEmExecucao->turnaround = (t+1) - processoEmExecucao->tempoEntrada;

				processosConcluidos++;
				processoEmExecucao = NULL;

		    } else{ // caso não tenha acabado execução
				printf("Processo %d possui %d segundos restantes de execucao.\n", processoEmExecucao->PID, processoEmExecucao->tempoExecRestante);
		    }	
	

	    	// 6. verificar se esse processo sofre IO no instante n da execução do processo
			if(processoEmExecucao != NULL){

				//se o processo ainda tiver I/O
				if(processoEmExecucao->qntdIO > processoEmExecucao->proxIO){

					printf("Processo %d possui %d io's restantes (momento %d do processo)\n", processoEmExecucao->PID, processoEmExecucao->qntdIO - processoEmExecucao->proxIO, processoEmExecucao->tempoExec - processoEmExecucao->tempoExecRestante);
					
					//se o processo chegar em um instante de I/O, inserir na respectiva fila
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
					
						//ao entrar em I/O, tiramos o processo da execução 
						processoEmExecucao = NULL;
                    
                	}
				}
            }	

		}
		
		//Função que printa as filas e seus processos a cada segundo de execução do escalonador
		printEstadoDaFila(filaAltaP, filaBaixaP, filaDiscoIO, filaFitaIO, filaImpressoraIO, processoEmExecucao);

	    t++;
		printf("%s", "--------------------------------------------------------------------------------\n");
		if (argc == 3 && strcmp(argv[2], "-s") == 0) getchar();
    }
	//Execução acaba
    
	printf("Todos os processos foram concluidos.\nTempo de execucao total: %d segundos\nTurnaround para cada processo:\n", t);
	for(int i = 0; i < numProcessos; i++){
		printf("\tProcesso %d: %d segundos\n", listaProcessos[i].PID, listaProcessos[i].turnaround);
	}

	// Removendo filas e listas
    apagarEstruturas(filaAltaP, filaBaixaP, filaDiscoIO, filaFitaIO, filaImpressoraIO, listaProcessos, numProcessos);
	return 0;
}

//função de inicialização das filas e listas
void inicializarEstruturas(FILA** filaAltaP, FILA** filaBaixaP, FILA** filaDiscoIO, FILA** filaFitaIO, FILA** filaImpressoraIO, Processo** listaProcessos, int numProcessos){
    if(initFila(filaAltaP) || initFila(filaBaixaP) || initFila(filaDiscoIO) || initFila(filaFitaIO) || initFila(filaImpressoraIO)){
        printf("Erro ao inicializar filas.\n");
        exit(-2);
    }
    
    *listaProcessos = (Processo*)malloc(numProcessos * sizeof(Processo));
    if (!*listaProcessos) {
        perror("Erro ao alocar memoria para os processos.\n");
        exit(-3);
    }
}

//função de remoção das filas e listas, liberando espaço de memória
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