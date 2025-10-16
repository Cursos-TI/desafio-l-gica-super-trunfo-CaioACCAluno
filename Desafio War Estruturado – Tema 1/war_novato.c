#include <stdio.h>  
#include <string.h> 

// Constante que define o número maximo de territorios a serem cadastrados
#define MAX_TERRITORIOS 5
#define TAMANHO_NOME 30
#define TAMANHO_COR 10
/**
 * Definição da estrutura (struct) Territorio.
 *
 */
struct Territorio {
    char nome[TAMANHO_NOME];  // Nome do territorio 
    char cor[TAMANHO_COR];   // Cor do exercito que ocupa o territorio 
    int tropas;     // Quantidade de tropas presentes no territorio
};

// --- Função Principal ---
int main() {
    // Declaração de um vetor de structs do tipo Territorio.
    struct Territorio mapa[MAX_TERRITORIOS];
    int i; // Variavel de controle para os laços (loops)

    printf("=============================================\n");
    printf("Sistema de Cadastro de Territorios - WAR\n");
    printf("=============================================\n");
    printf("Iniciando o cadastro de %d territorios...\n", MAX_TERRITORIOS);
    printf("---------------------------------------------\n");


    // O laço 'for' itera MAX_TERRITORIOS vezes (de 0 a 4) para preencher cada posição do vetor.
    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\nCadastro do Territorio #%d:\n", i + 1);

        // -- Nome do Territorio --
        printf("Digite o nome do territorio(max. 29 caracteres): ");
        // Usando scanf com %29s para evitar overflow
        if (scanf("%29s", mapa[i].nome) != 1) {
             printf("Erro na leitura do nome.\n");
             break; // Sai do loop em caso de erro
        }

        // -- Cor do Exercito --
        printf("Digite a cor do exercito: ");
        // Usando scanf com %9s para ler a string da cor.
        if (scanf("%9s", mapa[i].cor) != 1) {
             printf("Erro na leitura da cor.\n");
             break; // Sai do loop em caso de erro
        }

        // -- Quantidade de Tropas --
        printf("Digite a quantidade inicial de tropas: ");
        // Usando scanf para ler o inteiro.
        if (scanf("%d", &mapa[i].tropas) != 1) {
             printf("Erro na leitura de tropas. Certifique-se de digitar um número.\n");
             break; // Sai do loop em caso de erro
        }
    }


    printf("\n\n=============================================\n");
    printf("Cadastro Concluido! Exibindo Dados...\n");
    printf("=============================================\n");

    // Um segundo laço 'for' percorre o vetor para exibir os dados registrados.
    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("-> Territorio #%d\n", i + 1);
        printf("   Nome:   %s\n", mapa[i].nome);
        printf("   Cor:    %s\n", mapa[i].cor);
        printf("   Tropas: %d\n", mapa[i].tropas);
        printf("---------------------------------------------\n");
    }

    printf("\nProcesso finalizado com sucesso.\n");

    return 0; // Indica que o programa terminou corretamente
}