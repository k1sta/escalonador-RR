/*
comando de compilação:

gcc aoksdjasi odjasd 
*/

#include <stdio.h>
#include <stdbool.h>
#include "universal.h"
//PID, Tempo_Entrada, Tempo_Execução, Qntd_IOs, Tipo_IO, Inicio, Tempo_IO
//Consideramos o valor da fatia como 1 seg, ou seja, uma iteração do for

#define ARQ_INPUT "input.txt"

int main(void){
    // Declarando
    FILA* filaAltaP = (FILA*)malloc(sizeof(FILA));
    FILA* filaBaixaP = (FILA*)malloc(sizeof(FILA));
    FILA* filaDiscoIO = (FILA*)malloc(sizeof(FILA));
    FILA* filaFitaIO = (FILA*)malloc(sizeof(FILA));
    FILA* filaImpressoraIO = (FILA*)malloc(sizeof(FILA));
    Processo* listaProcessos = NULL;

    if (filaAltaP == NULL || filaBaixaP == NULL || filaDiscoIO == NULL || filaFitaIO == NULL || filaImpressoraIO == NULL) {
        printf("Erro ao alocar memória para as filas.\n");
        return -1;
    }

    // Inicializando filas
    initFila(filaAltaP); initFila(filaBaixaP); initFila(filaDiscoIO); initFila(filaFitaIO); initFila(filaImpressoraIO);




    // Removendo filas
    deletarFila(filaAltaP); deletarFila(filaBaixaP); deletarFila(filaDiscoIO); deletarFila(filaFitaIO); deletarFila(filaImpressoraIO);
}


