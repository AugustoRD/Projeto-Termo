#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "termo.h"

void normalizarString(char *str) {
	int i;
    for (i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}


int empilha(Pilha *p, char *palavra) {
    Nodo *novoNo = (Nodo*)malloc(sizeof(Nodo));
    if (!novoNo){
		return FALHA;}

    strcpy(novoNo->palavra, palavra);
    novoNo->proximo = p->topo;
    p->topo = novoNo;
    p->tamanho++;
    return SUCESSO;
}


int insere(Fila *f, char *palavra) {
    Nodo *novoNo = (Nodo*)malloc(sizeof(Nodo));
    if (!novoNo){
		return FALHA;}

    strcpy(novoNo->palavra, palavra);
    novoNo->proximo = NULL;
    if (f->fim == NULL) {
        f->inicio = f->fim = novoNo;
    } else {
        f->fim->proximo = novoNo;
        f->fim = novoNo;
    }
    return SUCESSO;
}


void imprimirFeedback(char *tentativa, char *palavraSecreta) {
    char feedback[TAMANHO_PALAVRA];
    memset(feedback, 0, sizeof(feedback));
    
    int contagemLetrasSecretas[26] = {0};
    int i;
    for (i = 0; i < TAMANHO_PALAVRA; i++) {
        contagemLetrasSecretas[palavraSecreta[i] - 'A']++;
    }

    for (i = 0; i < TAMANHO_PALAVRA; i++) {
        if (tentativa[i] == palavraSecreta[i]) {
            feedback[i] = 'G';
            contagemLetrasSecretas[tentativa[i] - 'A']--;
        }
    }

    for (i = 0; i < TAMANHO_PALAVRA; i++) {
        if (feedback[i] == 0) {
            if (contagemLetrasSecretas[tentativa[i] - 'A'] > 0) {
                feedback[i] = 'Y';
                contagemLetrasSecretas[tentativa[i] - 'A']--;
            } else {
                feedback[i] = 'X';
            }
        }
    }

    printf("   ");
    for (i = 0; i < TAMANHO_PALAVRA; i++) {
        if (feedback[i] == 'G') printf(GREEN " %c " RESET_COLOR, tentativa[i]);
        else if (feedback[i] == 'Y') printf(YELLOW " %c " RESET_COLOR, tentativa[i]);
        else printf(GRAY " %c " RESET_COLOR, tentativa[i]);
    }
    printf("\n\n");
}


void criaPilha(Pilha *p) {
    p->topo = NULL;
    p->tamanho = 0;
}


void carregarPalavras(Pilha *pilha, char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de palavras");
        return;
    }
    char buffer[10];
    while (fgets(buffer, sizeof(buffer), arquivo)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == TAMANHO_PALAVRA) {
            normalizarString(buffer);
            empilha(pilha, buffer); 
        }
    }
    fclose(arquivo);
}


void liberarPilha(Pilha *pilha) {
    if (pilha) {
        Nodo *atual = pilha->topo;
        while (atual != NULL) {
            Nodo *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        pilha->topo = NULL;
        pilha->tamanho = 0;
    }
}


void criaFila(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
}


void prepararFilaDeJogo(Fila *filaJogo, Pilha *dicionario) {
    if (dicionario == NULL || dicionario->tamanho == 0) return;

    char **arrayDePalavras = (char**)malloc(dicionario->tamanho * sizeof(char*));
    if (!arrayDePalavras){
		return;}

    Nodo *atual = dicionario->topo;
    int i;
    for (i = 0; i < dicionario->tamanho; i++) {
        arrayDePalavras[i] = atual->palavra;
        atual = atual->proximo;
    }

    for (i = dicionario->tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = arrayDePalavras[i];
        arrayDePalavras[i] = arrayDePalavras[j];
        arrayDePalavras[j] = temp;
    }

    for (i = 0; i < dicionario->tamanho; i++) {
        insere(filaJogo, arrayDePalavras[i]); 
    }
    free(arrayDePalavras);
}


int estaVazia(Fila f) {
    return (f.inicio == NULL);
}


int retira(Fila *f, char **palavra) {
    if (f == NULL || f->inicio == NULL) {
        return FALHA;
    }
    
    Nodo *temp = f->inicio;
    
    *palavra = (char*)malloc(TAMANHO_PALAVRA + 1);
    if (!(*palavra)) {
        return FALHA;
    }
    
    strcpy(*palavra, temp->palavra);

    f->inicio = f->inicio->proximo;
    if (f->inicio == NULL) {
        f->fim = NULL;
    }
    
    free(temp);
    return SUCESSO;
}


void liberarFila(Fila *f) {
    if (f) {
        char *p;
        while (retira(f, &p) == SUCESSO) {
            free(p);
        }
    }
}


void jogarRodada(char *palavraSecreta, Pilha *dicionario) {
    char tentativa[100];
    int numTentativas = 0;
    int acertou = 0;

    while (numTentativas < MAX_TENTATIVAS && !acertou) {
        printf("Tentativa %d de %d. Digite sua palavra: ", numTentativas + 1, MAX_TENTATIVAS);
        if (scanf("%s", tentativa) != 1) {
            while (getchar() != '\n' && getchar() != EOF);
            continue;
        }

        if (strlen(tentativa) != TAMANHO_PALAVRA) {
            printf("Palavra invalida! Deve ter %d letras.\n\n", TAMANHO_PALAVRA);
            continue;
        }
        normalizarString(tentativa);

        int palavraValida = 0;
        Nodo *atual = dicionario->topo;
        while (atual != NULL) {
            if (strcmp(atual->palavra, tentativa) == 0) {
                palavraValida = 1;
                break;
            }
            atual = atual->proximo;
        }
        if (!palavraValida) {
            printf("Essa palavra nao esta no nosso dicionario.\n\n");
            continue;
        }

        numTentativas++;
        imprimirFeedback(tentativa, palavraSecreta);
        if (strcmp(tentativa, palavraSecreta) == 0) {
            acertou = 1;
        }
    }

    if (acertou) {
        printf("Parabens! Voce acertou a palavra '%s' em %d tentativas!\n", palavraSecreta, numTentativas);
    } else {
        printf("Que pena! Voce nao conseguiu adivinhar. A palavra era '%s'.\n", palavraSecreta);
    }
}
