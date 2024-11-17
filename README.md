# escalonador-RR

Simulador de escalonador usando o algoritmo de Round Robin com Feedback

## Descrição

Este projeto é o trabalho final da disciplina de Arquitetura de Computadores e Sistemas Operacionais da Universidade Federal do Rio de Janeiro (UFRJ). O objetivo do projeto é implementar um simulador de escalonador de processos utilizando o algoritmo de Round Robin com Feedback.

## Estrutura do Projeto

O projeto é composto pelos seguintes arquivos:

- [`main.c`](main.c): Contém a função principal e a lógica do escalonador.
- [`fila.c`](fila.c): Implementa as funções para manipulação das filas de processos.
- [`leitura.c`](leitura.c): Contém as funções para leitura dos processos a partir de arquivos de entrada.
- [`impressao.c`](impressao.c): Contém as funções para impressão do estado das filas e dos processos.
- [`teste_impresso.c`](teste_impresso.c): Arquivo de teste para verificar a impressão dos processos e suas operações de I/O.
- [`universal.h`](universal.h): Cabeçalho com as definições das estruturas e funções utilizadas no projeto.
- [`input.txt`](input.txt), [`input2.txt`](input2.txt), [`input3.txt`](input3.txt): Arquivos de entrada com os dados dos processos a serem simulados.
- [`README.md`](README.md): Este arquivo de descrição do projeto.

## Compilação e Execução

Para compilar o projeto, utilize o comando:

```sh
gcc -o escalonador main.c fila.c leitura.c impressao.c
```

Para executar o simulador, utilize o comando:

```sh
./escalonador <input_file>
```
Onde <input_file> é o caminho para um dos arquivos de entrada (`input.txt`, `input2.txt`, `input3.txt`).

## Funcionalidades
- Leitura dos processos a partir de um arquivo de entrada.
- Simulação do escalonamento de processos utilizando o algoritmo de Round Robin com Feedback.
- Manipulação de filas de alta e baixa prioridade, além de filas de I/O para diferentes tipos de dispositivos.
- Impressão do estado das filas e dos processos durante a simulação.

## Estruturas de Dados
- `Processo`: Estrutura que representa um processo, contendo informações como PID, tempo de entrada, tempo de execução, quantidade de operações de I/O, entre outros.
- `FILA`: Estrutura que representa uma fila de processos, contendo ponteiros para o início e o fim da fila.
- `Elemento`: Estrutura que representa um elemento de uma fila, contendo um ponteiro para o processo e um ponteiro para o próximo elemento.
- `IO`: Estrutura que representa uma operação de I/O, contendo informações sobre o tipo de dispositivo e o tempo de início da operação.

## Autores
Este projeto foi desenvolvido pelos alunos da disciplina de Arquitetura de Computadores e Sistemas Operacionais da UFRJ.

## Licença
Este projeto é de uso acadêmico e não possui uma licença específica.