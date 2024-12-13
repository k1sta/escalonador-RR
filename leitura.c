#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "universal.h"

#define TAM_MAX_LINHA 256


// Função auxiliar para realizar o sort dos IO's
int compararIOs(const void* a, const void* b) {
    IO* ioA = (IO*)a;
    IO* ioB = (IO*)b;
    return (ioA->inicio - ioB->inicio);
}

//função para contagem de Processos no arquivo input
//conta as linhas do arquivo e diminui 3, pois temos 2 cabeçalhos 
//e uma linha para input dos tempos de cada tipo de I/O
int contarProcessos(const char* arqNome) {
    FILE* arq = fopen(arqNome, "r");
    if (!arq){
        perror("Erro ao abrir o arquivo");
        return -2;
    }  
    
    int qntLinhas = -2;
    char ch;

    while ((ch = fgetc(arq)) != EOF) {
        if (ch == '\n') {
            qntLinhas++;
        }
    }

    fclose(arq);

    return qntLinhas;
}

//função que lê os processos do arquivo de input e os carrega num vetor de processos
int lerProcessos(const char* arqNome, Processo** processos, int numProcessos){
    
    FILE* arq = fopen(arqNome, "r");
    if (!arq){
        perror("Erro ao abrir o arquivo");
        return -2;
    }  

    // cria o vetor de char para as linhas
    char linha[TAM_MAX_LINHA];

    // cria as variáveis para o tempo de IO de cada tipo
    int tempoIOFita, tempoIODisco, tempoIOImpressora;


    // pula o cabeçalho de IO e coleta os tempos de IO
    fgets(linha, sizeof(linha), arq);
    fgets(linha, sizeof(linha), arq);
    sscanf(linha, "%d %d %d", &tempoIOFita, &tempoIODisco, &tempoIOImpressora);
    printf("Tempo de IO para a fita: %d | Tempo de IO para o disco: %d | Tempo de IO para a impressora: %d\n", tempoIOFita, tempoIODisco, tempoIOImpressora);
    
    // pula o cabeçalho dos processos
    fgets(linha, sizeof(linha), arq);

    int i = 0;

    while (i < numProcessos && fgets(linha, sizeof(linha), arq)) {
        // Inicializando processo
        int numIO;
        (*processos)[i].PID = i + 1;
        sscanf(linha, "%d %d %d", &((*processos)[i].tempoEntrada), &((*processos)[i].tempoExec), &numIO);
        (*processos)[i].tempoExecRestante = (*processos)[i].tempoExec;
        (*processos)[i].proxIO = 0;
        (*processos)[i].turnaround = -1;

        // Aloca os IO's do processo
        (*processos)[i].ios = (IO*)malloc(numIO * sizeof(IO));

        // Pega os IO's do processo no arquivo e os inicializa
        if (numIO){
            char* linha_io = linha;
            for (int j = 0; j < 3; j++){
                linha_io = strchr(linha_io, ' ') + 1;
            }
            for (int j = 0; j < numIO; j++) {
                sscanf(linha_io, "%c %d", &((*processos)[i].ios[j].tipo), &((*processos)[i].ios[j].inicio));
                switch((*processos)[i].ios[j].tipo){
                    case 'F':
                        (*processos)[i].ios[j].tempoExecRestante = tempoIOFita;
                        break;
                    case 'I':
                        (*processos)[i].ios[j].tempoExecRestante = tempoIOImpressora;
                        break;
                    case 'D':
                        (*processos)[i].ios[j].tempoExecRestante = tempoIODisco;
                        break;
                }
		        
                for (int k = 0; k < 2; k++) 
                    linha_io = strchr(linha_io, ' ') + 1;
            }

            qsort((*processos)[i].ios, numIO, sizeof(IO), compararIOs);

        }
        
        (*processos)[i].qntdIO = numIO;

        i++;
    }

    fclose(arq);

    return 0;
}
