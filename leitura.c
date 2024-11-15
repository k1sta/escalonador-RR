#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "universal.h"

#define TAM_MAX_LINHA 256

int contarLinhas(const char* arqNome) {
    FILE* arq = fopen(arqNome, "r");
    if (!arq){
        perror("Erro ao abrir o arquivo");
        return -2;
    }  
    
    int qntLinhas = 0;
    char ch;
    while ((ch = fgetc(arq)) != EOF) {
        if (ch == '\n') {
            qntLinhas++;
        }
    }

    fclose(arq);

    return qntLinhas;
}

int lerProcessos(const char* arqNome, Processo** processos, int numProcessos){
    FILE* arq = fopen(arqNome, "r");
    if (!arq){
        perror("Erro ao abrir o arquivo");
        return -2;
    }  
    
    // cria vetor de processos
    *processos = (Processo*)malloc(numProcessos * sizeof(Processo));
    if (!processos) {
            perror("Erro ao alocar memória para oS processoS.\n");
            fclose(arq);
            return -3;
        }

    // cria o vetor de char para as linhas e despreza o cabeçalho
    char linha[TAM_MAX_LINHA];
    fgets(linha, sizeof(linha), arq);

    int i = 0;

    while (i < numProcessos && fgets(linha, sizeof(linha), arq)) {
        //printf("Entrando no processo %d\n", i+1); // DEBUG
        int numIO;
        sscanf(linha, "%d %d %d %d", &((*processos)[i].PID), &((*processos)[i].tempoEntrada), &((*processos)[i].tempoExec), &numIO);
	(*processos)[i].tempoExecRestante = (*processos)[i].tempoExec;
	(*processos)[i].proxIO = 0;



        // Aloca os IO's
        (*processos)[i].ios = (IO*)malloc(numIO * sizeof(IO));

        if (numIO){
            char* linha_io = linha;
            for (int j = 0; j < 4; j++){
                linha_io = strchr(linha_io, ' ') + 1;
            }
            for (int j = 0; j < numIO; j++) {
                sscanf(linha_io, "%c %d %d", &((*processos)[i].ios[j].tipo), &((*processos)[i].ios[j].inicio), &((*processos)[i].ios[j].tempoExec));
                //printf("%c %d %d\n", (*processos)[i].ios[j].tipo, (*processos)[i].ios[j].inicio, (*processos)[i].ios[j].tempoExec); // DEBUG
               	(*processos)[i].ios[j].tempoExecRestante = (*processos)[i].ios[j].tempoExec;

		for (int k = 0; k < 3; k++)
                    linha_io = strchr(linha_io, ' ') + 1;

            }
        }

        (*processos)[i].qntdIO = numIO;


        i++;
    }

    fclose(arq);

    return 0;
}
