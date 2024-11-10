#include <stdio.h>
#include <stdbool.h>
#include "universal.h"


void initFila(FILA* f)
{
    // inicia os vetores da fila como nulo
    f->inicio = NULL;
    f->fim = NULL;
}

bool inserirFila(FILA* f, int chaveNova)
{
    Elemento* novo = (Elemento*)malloc(sizeof(Elemento));
    
    // confirma se espaço foi alocado
    if (!novo) {
        return false;
    }

    // configura novo elemento
    novo->chave = chaveNova;
    novo->prox = NULL;

    // se fila vazia, insere no inicio
    if (f->inicio == NULL){
        f->inicio = novo;
    }
    else { 
        f->fim->prox = novo;
    }
    
    f->fim = novo; 
    return true;
}

int removerFila(FILA *f)
{
    Elemento* aux = f->inicio;
    if (!aux->prox){
        f->inicio = NULL;
    } else{
        f->inicio = aux->prox;
    }

    int pid = aux->chave;
    free(aux);
    return pid;
}

void deletarFila(FILA *f)
{
    Elemento* aux = f->inicio;
    while(aux != NULL){
        Elemento* apagar = aux;
        aux = aux->prox;
        free(apagar);
    }
    free(f);
}