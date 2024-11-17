#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "universal.h"


int initFila(FILA** f)
{
    if (!*f) {
        *f = (FILA*) malloc (sizeof(FILA));
        if (!*f) {
            printf("Erro na alocação da fila");
            return -1;
        }
    }
    
    // inicia os vetores da fila como nulo
    (*f)->inicio = NULL;
    (*f)->fim = NULL;
    return 0;
}

bool inserirFila(FILA* f, Processo* chaveNova)
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

Processo* removerFila(FILA *f)
{
    if (f->inicio == NULL){
        return NULL;
    }
    Elemento* aux = f->inicio;
    if (!aux->prox){
        f->inicio = NULL;
    } else{
        f->inicio = aux->prox;
    }

    Processo* pid = aux->chave;
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
