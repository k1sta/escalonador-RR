#ifndef DADOS_H // começa bloco caso DADOS_H não esteja definido

#define DADOS_H
#include <stdio.h>
#include <stdbool.h>

// Estrutura para I/O
typedef struct {
    char tipo; // Fita magnética (F) | Disco (D) | Impressora (I)
    int inicio;
    int tempoExecRestante;
} IO;

// Estrutura para Processo
typedef struct {
    int PID;
    int tempoEntrada;
    int tempoExec;
    int tempoExecRestante;
    int turnaround;
    int qntdIO;
    int proxIO; //indice do próximo IO a ser executado
    IO* ios; //vetor de IOs
} Processo;

// Estruturas para fila
typedef struct Elemento {
    Processo* chave;
    struct Elemento* prox;
} Elemento;

typedef struct FILA{
    Elemento* inicio;
    Elemento* fim;
} FILA;

// funções para as filas

int initFila(FILA** f);

bool inserirFila(FILA* f, Processo* chaveNova);

Processo* removerFila(FILA *f);

void deletarFila(FILA *f);

void decrementarTemposFila(FILA *f);

// funções para a leitura 

int compararIOs(const void* a, const void* b);

int contarProcessos(const char* arqNome);

int lerProcessos(const char* arqNome, Processo** processos, int numProcessos);

// funções para a impressão

void imprimirTabelaProcessos(Processo* processos, int numProcessos);

void imprimirTabelaIO(Processo* processos, int numProcessos);

void printEstadoDasFilas(FILA* filaAltaP, FILA* filaBaixaP, FILA* filaDiscoIO, FILA* filaFitaIO, FILA* filaImpressoraIO, Processo* processoEmExecucao);

#endif
