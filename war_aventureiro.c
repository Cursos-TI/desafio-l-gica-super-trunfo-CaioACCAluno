#include <stdio.h>
#include <stdlib.h> // Para calloc, free
#include <string.h> // Para strcpy, strcmp
#include <time.h>   // Para srand, time

#define TAMANHO_NOME 30
#define TAMANHO_COR 10

/**
 * Definicao da estrutura (struct) Territorio.
 */
struct Territorio {
    char nome[TAMANHO_NOME];
    char cor[TAMANHO_COR];
    int tropas;
};

// =================================================================
// FUNCOES AUXILIARES
// =================================================================

// Rola um dado de 6 faces (1 a 6).
int rola_dado() {
    return (rand() % 6) + 1;
}

// Libera a memoria alocada dinamicamente pelo calloc.
void liberar_memoria(struct Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemoria liberada com sucesso.\n");
    }
}

/**
 * Exibe os dados de todos os territorios.
 */
void exibir_territorios(struct Territorio* mapa, int total_territorios) {
    if (mapa == NULL || total_territorios <= 0) {
        printf("\nMapa vazio ou invalido.\n");
        return;
    }

    printf("\n=============================================\n");
    printf("     ESTADO ATUAL DO MAPA \n");
    printf("=============================================\n");

    for (int i = 0; i < total_territorios; i++) {
        // Acesso via notacao de ponteiro
        printf("-> [%d] Territorio: %s\n", i, (mapa + i)->nome);
        printf("   Cor: %s\n", (mapa + i)->cor);
        printf("   Tropas: %d\n", (mapa + i)->tropas);
        printf("---------------------------------------------\n");
    }
}

// =================================================================
// FUNCOES DE SIMULACAO
// =================================================================

/**
 * Simula uma batalha entre dois territorios, atualizando as tropas.
 * Ao fim da batalha, exibe o estado atualizado dos dois territorios.
 */
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    if (atacante->tropas <= 1) {
        printf("\nAtaque cancelado: Atacante deve manter pelo menos 1 tropa na origem.\n");
        return;
    }

    int dado_ataque = rola_dado();
    int dado_defesa = rola_dado();

    printf("\nSIMULACAO DE ATAQUE INICIADA\n");
    printf("   Atacante (%s - %s) rolou: %d\n", atacante->nome, atacante->cor, dado_ataque);
    printf("   Defensor (%s - %s) rolou: %d\n", defensor->nome, defensor->cor, dado_defesa);

    if (dado_ataque > dado_defesa) {
        // VITORIA DO ATACANTE: Conquista o territorio e move metade das tropas.
        int tropas_a_mover = atacante->tropas / 2;
        
        strcpy(defensor->cor, atacante->cor);
        
        atacante->tropas -= tropas_a_mover;
        defensor->tropas = tropas_a_mover;

        printf("\nVITORIA! %s conquistou %s e moveu %d tropas.\n", atacante->nome, defensor->nome, tropas_a_mover);

    } else {
        // DERROTA ou EMPATE: Atacante perde 1 tropa.
        atacante->tropas -= 1;
        printf("\nDEFENSOR RESISTIU! %s perdeu 1 tropa em %s.\n", atacante->nome, defensor->nome);
    }
    
  
    printf("\n--- ESTADO ATUALIZADO DOS ENVOLVIDOS ---\n");
    printf("-> Territorio: %s (Tropas: %d, Cor: %s)\n", atacante->nome, atacante->tropas, atacante->cor);
    printf("-> Territorio: %s (Tropas: %d, Cor: %s)\n", defensor->nome, defensor->tropas, defensor->cor);
    printf("----------------------------------------\n");
}

/**
 * Gerencia a alocacao dinamica e o cadastro.
 */
struct Territorio* cadastrar_territorios(int num_territorios) {
    // Alocacao dinamica com calloc
    struct Territorio* mapa = (struct Territorio*)calloc(num_territorios, sizeof(struct Territorio));

    if (mapa == NULL) {
        printf("\nERRO: Falha na alocacao de memoria.\n");
        return NULL;
    }

    printf("       Iniciando o cadastro de %d territorios...\n", num_territorios);

    for (int i = 0; i < num_territorios; i++) {
        printf("\nCadastro do Territorio #%d:\n", i);
        
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
        printf("Digite a quantidade inicial de tropas: (min. 1)");
        // Usando scanf para ler o inteiro.
        if (scanf("%d", &mapa[i].tropas) != 1) {
             printf("Erro na leitura de tropas. Certifique-se de digitar um número.\n");
             break; // Sai do loop em caso de erro
        }
        }
    }
    
    printf("\nCadastro concluido.\n");
    return mapa;
}

// =================================================================
// FUNCAO PRINCIPAL
// =================================================================

int main() {
    // Inicializa o gerador de numeros aleatorios
    srand(time(NULL));
    
    struct Territorio* mapa = NULL;
    int num_territorios;
    int id_ataque, id_defesa;
    char continuar_ataque;

    printf("=============================================\n");
    printf("     Sistema de Batalha - WAR (Nivel 2)\n");
    printf("=============================================\n");

    // 1. Alocacao e Cadastro
    printf("   Digite a quantidade de territorios (Min. 2): ");
    if (scanf("%d", &num_territorios) != 1 || num_territorios < 2) {
        printf("Quantidade invalida. Encerrando.\n");
        return 1;
    }
    
    mapa = cadastrar_territorios(num_territorios);

    if (mapa == NULL) return 1;

    // 2. Loop de Ataque
    do {
        // Limpa o buffer de entrada
        while (getchar() != '\n');
        
        exibir_territorios(mapa, num_territorios);
        
        printf("\n[SISTEMA DE ATAQUE]\n");
        printf("ID do ATACANTE [0 a %d]: ", num_territorios - 1);
        if (scanf("%d", &id_ataque) != 1 || id_ataque < 0 || id_ataque >= num_territorios) break;
        
        printf("ID do DEFENSOR [0 a %d]: ", num_territorios - 1);
        if (scanf("%d", &id_defesa) != 1 || id_defesa < 0 || id_defesa >= num_territorios || id_defesa == id_ataque) break;

        struct Territorio* atacante = (mapa + id_ataque);
        struct Territorio* defensor = (mapa + id_defesa);

        // Validacao: nao pode atacar territorio da mesma cor
        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("\nNao e possivel atacar seu proprio territorio!\n");
        } else {
            // Chamada da funcao ATACAR com os IDs para a exibicao
            atacar(atacante, defensor, id_ataque, id_defesa);
        }
        
        printf("\nDeseja realizar outro ataque? (S/N): ");
        while (getchar() != '\n');
        scanf("%c", &continuar_ataque);
        
    } while (continuar_ataque == 'S' || continuar_ataque == 's');


    // 3. Limpeza
    liberar_memoria(mapa);
    
    printf("\nProcesso finalizado com sucesso.\n");

    return 0;
}