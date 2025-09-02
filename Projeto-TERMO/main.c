#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "termo.h"

int main() {
    srand(time(NULL));
    system("clear || cls");

    printf("==============================\n");
    printf("      BEM-VINDO AO TERMO!     \n");
    printf("==============================\n\n");
    printf("Regras:\n");
    printf("- Tente adivinhar a palavra de %d letras.\n", TAMANHO_PALAVRA);
    printf("- Voce tem %d tentativas.\n", MAX_TENTATIVAS);
    printf("- " GREEN "  Verde  " RESET_COLOR " significa letra certa no lugar certo.\n");
    printf("- " YELLOW " Amarelo " RESET_COLOR " significa letra certa no lugar errado.\n");
    printf("- " GRAY "  Cinza  " RESET_COLOR " significa que a letra nao existe na palavra.\n\n");

    Pilha dicionario;
    criaPilha(&dicionario);
    
    carregarPalavras(&dicionario, "palavras.txt");

    if (dicionario.tamanho == 0) {
        printf("\nNenhuma palavra encontrada no dicionario. Encerrando.\n");
        return 1;
    }
    printf("%d palavras carregadas para o dicionario.\n", dicionario.tamanho);
    printf("Pressione ENTER para iniciar...");
    getchar();

    Fila filaDePalavras;
    criaFila(&filaDePalavras);
    
    prepararFilaDeJogo(&filaDePalavras, &dicionario);

    int rodada_atual = 1;
    
    while (!estaVazia(filaDePalavras)) {
        char* palavraSecreta;
        
        if (retira(&filaDePalavras, &palavraSecreta) == SUCESSO) {
            system("clear || cls");
            printf("--- PALAVRA %d de %d ---\n\n", rodada_atual, dicionario.tamanho);
            
            jogarRodada(palavraSecreta, &dicionario);
            free(palavraSecreta);

            if (estaVazia(filaDePalavras)) {
                break;
            }

            printf("\nDeseja jogar a proxima rodada? (S/N): ");
            char resposta = ' ';
            scanf(" %c", &resposta);

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            if (resposta == 'N' || resposta == 'n') {
                printf("\nAte a proxima!\n");
                break;
            }
            rodada_atual++;
        }
    }
    
    if (estaVazia(filaDePalavras)) {
        printf("\nParabens! Voce zerou o jogo e jogou todas as %d palavras!\n", dicionario.tamanho);
    }
    
    liberarPilha(&dicionario);
    liberarFila(&filaDePalavras);

    return 0;
}
