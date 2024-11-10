#ifndef DADOS_H // começa bloco caso DADOS_H não esteja definido

#define DADOS_H
#include <stdio.h>
#include <stdbool.h>

// structs

typedef struct {
    char tipo; // Fita magnética (F) | Disco (D) | Impressora (I)
    int inicio;
    int tempoExec;
} IO;

typedef struct {
    int PID;
    int tempoEntrada;
    int tempoExec;
    int qntdIO;
    IO* ios;
} Processo;

// Estruturas para 
typedef struct{
    int chave;
    struct Elemento* prox;
} Elemento;

typedef struct{
    Elemento* inicio;
    Elemento* fim;
} FILA;

// funções para as filas

void initFila(FILA* f);

bool inserirFila(FILA* f, int chaveNova);

void removerFila(FILA *f);

void deletarFila(FILA *f);

// funções para a leitura 


int contarLinhas(const char* arqNome);

int lerProcessos(const char* arqNome, Processo** processos, int numProcessos);

#endif