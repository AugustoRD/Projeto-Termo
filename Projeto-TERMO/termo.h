#ifndef TERMO_H
#define TERMO_H

#include <stdlib.h>

#define TAMANHO_PALAVRA 5
#define MAX_TENTATIVAS 6
#define SUCESSO 1
#define FALHA 0

#define RESET_COLOR   "\x1b[0m"
#define GREEN         "\x1b[42m"
#define YELLOW        "\x1b[43m"
#define GRAY          "\x1b[100m"

typedef struct Nodo {
    char palavra[TAMANHO_PALAVRA + 1];
    struct Nodo *proximo;
} Nodo;

typedef struct {
    Nodo *topo;
    int tamanho;
} Pilha;

typedef struct {
    Nodo *inicio;
    Nodo *fim;
} Fila;

void criaPilha(Pilha *p);
int empilha(Pilha *p, char *palavra); 
void liberarPilha(Pilha *pilha);

void criaFila(Fila *f);
int insere(Fila *f, char *palavra); 
int retira(Fila *f, char **palavra);
int estaVazia(Fila f);
void liberarFila(Fila *fila);

void carregarPalavras(Pilha *pilha, char *nomeArquivo);
void prepararFilaDeJogo(Fila *filaJogo, Pilha *dicionario);
void jogarRodada(char *palavraSecreta, Pilha *dicionario);

void normalizarString(char *str);
void imprimirFeedback(char *tentativa, char *palavraSecreta);


#endif
