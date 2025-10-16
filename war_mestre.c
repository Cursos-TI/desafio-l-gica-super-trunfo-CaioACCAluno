#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>   

#define TAMANHO_NOME_TERRITORIO 30
#define TAMANHO_COR 10
#define TAMANHO_MISSAO 100
#define MAX_JOGADORES 2

// =================================================================
// ESTRUTURAS DE DADOS
// =================================================================

struct Territorio {
    char nome[TAMANHO_NOME_TERRITORIO];
    char cor[TAMANHO_COR];
    int tropas;
};

struct Jogador {
    int id;
    char cor[TAMANHO_COR];
    char* missao; // Armazenamento dinamico
    int missoes_cumpridas;
};

// =================================================================
// VARIAVEIS GLOBAIS (Requisito: Apenas 3 Missoes)
// =================================================================

char* MISSOES_ESTRATEGICAS[] = {
    "Conquistar 4 territorios.",                   
    "Eliminar todas as tropas da cor Azul.",       
    "Possuir mais que a metade dos territorios.",  
};
int TOTAL_MISSOES = 3;

// =================================================================
// FUNCOES DE MISSAO E JOGADOR
// =================================================================

/**
 * @brief Sorteia e copia uma missao para o espaco alocado do jogador.
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice_sorteado = rand() % totalMissoes;
    char* missao_sorteada = missoes[indice_sorteado];

    // Requisito: Copia para a variavel de missao usando strcpy
    strcpy(destino, missao_sorteada);
}

/**
 * @brief Avalia se a missao do jogador foi cumprida utilizando logica baseada
 * na string da missao e no estado atual do mapa.
 */
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, char* cor_jogador) {
    int territorios_do_jogador = 0;
    
    for (int i = 0; i < tamanho; i++) {
        if (strcmp((mapa + i)->cor, cor_jogador) == 0) {
            territorios_do_jogador++;
        }
    }
    
    // Logica 1: Conquistar 4 territorios
    if (strstr(missao, "Conquistar 4 territorios") != NULL) {
        if (territorios_do_jogador >= 4) return 1;
    } 
    // Logica 2: Possuir mais que a metade dos territorios
    else if (strstr(missao, "metade dos territorios") != NULL) {
        if (territorios_do_jogador > (tamanho / 2)) return 1;
    }
    // Logica 3: Eliminacao (A cor do jogador e 'Vermelho' ou 'Azul')
    else if (strstr(missao, "Eliminar todas as tropas da cor") != NULL) {
        char cor_alvo[TAMANHO_COR] = "";
        
        // Define qual a cor alvo (a cor do adversario)
        if (strcmp(cor_jogador, "Vermelho") == 0) {
            strcpy(cor_alvo, "Azul");
        } else if (strcmp(cor_jogador, "Azul") == 0) {
            strcpy(cor_alvo, "Vermelho");
        }

        if (strlen(cor_alvo) > 0) {
            int cor_alvo_existe = 0;
            for (int i = 0; i < tamanho; i++) {
                if (strcmp((mapa + i)->cor, cor_alvo) == 0) {
                    cor_alvo_existe = 1;
                    break;
                }
            }
            // Se a cor alvo nao existe (0), a missao foi cumprida.
            if (cor_alvo_existe == 0) return 1;
        }
    }
    
    return 0; // Missao nao cumprida
}

/**
 * @brief Exibe a missao do jogador. (Requisito: Passada por valor para exibicao).
 */
void exibirMissao(char* missao, int id_jogador, char* cor) {
    printf("\nMISSAO DO JOGADOR %d (%s):\n", id_jogador, cor);
    printf("-> %s\n", missao);
    printf("---------------------------------------------\n");
}

// =================================================================
// FUNCOES DE MAPA E ATAQUE
// =================================================================

int rola_dado() {
    return (rand() % 6) + 1;
}

/**
 * @brief Libera toda a memoria alocada dinamicamente.
 */
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores) {
    if (mapa != NULL) {
        free(mapa);
    }
    if (jogadores != NULL) {
        for (int i = 0; i < MAX_JOGADORES; i++) {
            // Requisito: Utilizar free() no final para evitar vazamentos de memoria.
            if (jogadores[i].missao != NULL) {
                free(jogadores[i].missao);
            }
        }
        free(jogadores);
    }
    printf("\nMemoria liberada com sucesso.\n");
}

/**
 * @brief Exibe o estado atual de todos os territorios. (exibirMapa)
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
        printf("-> [%d] Territorio: %s\n", i, (mapa + i)->nome);
        printf("   Cor: %s\n", (mapa + i)->cor);
        printf("   Tropas: %d\n", (mapa + i)->tropas);
        printf("---------------------------------------------\n");
    }
}

/**
 * @brief Simula uma batalha entre dois territorios. (atacar)
 */
void atacar(struct Territorio* atacante, struct Territorio* defensor, int id_ataque, int id_defesa) {
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
        // Vitoria - transfere cor e metade das tropas
        int tropas_a_mover = atacante->tropas / 2;
        
        strcpy(defensor->cor, atacante->cor);
        
        atacante->tropas -= tropas_a_mover;
        defensor->tropas = tropas_a_mover;

        printf("\nVITORIA! %s conquistou %s e moveu %d tropas.\n", atacante->nome, defensor->nome, tropas_a_mover);

    } else {
        // Derrota - atacante perde 1 tropa
        atacante->tropas -= 1;
        printf("\nDEFENSOR RESISTIU! %s perdeu 1 tropa em %s.\n", atacante->nome, defensor->nome);
    }
    
    printf("\n--- ESTADO ATUALIZADO DOS ENVOLVIDOS ---\n");
    printf("-> [%d] Territorio: %s (Tropas: %d, Cor: %s)\n", id_ataque, atacante->nome, atacante->tropas, atacante->cor);
    printf("-> [%d] Territorio: %s (Tropas: %d, Cor: %s)\n", id_defesa, defensor->nome, defensor->tropas, defensor->cor);
    printf("----------------------------------------\n");
}

/**
 * @brief Gerencia a alocacao dinamica e o cadastro de territorios.
 */
struct Territorio* cadastrar_territorios(int num_territorios, struct Jogador* jogadores) {
    // Alocacao de territorios (Requisito: calloc)
    struct Territorio* mapa = (struct Territorio*)calloc(num_territorios, sizeof(struct Territorio));

    if (mapa == NULL) {
        printf("\nERRO: Falha na alocacao de memoria para o mapa.\n");
        return NULL;
    }

    printf("       Iniciando o cadastro de %d territorios...\n", num_territorios);
    int territorios_por_jogador = num_territorios / MAX_JOGADORES;

    for (int i = 0; i < num_territorios; i++) {
        int jogador_indice = i / territorios_por_jogador;
        if (jogador_indice >= MAX_JOGADORES) {
            jogador_indice = MAX_JOGADORES - 1;
        }

        printf("\nCadastro do Territorio #%d:\n", i);
        
        printf("   Nome: ");
        if (scanf("%29s", (mapa + i)->nome) != 1) return NULL; 

        strcpy((mapa + i)->cor, jogadores[jogador_indice].cor);
        printf("   Cor: %s (Atribuida ao Jogador %d)\n", (mapa + i)->cor, jogadores[jogador_indice].id);

        printf("   Tropas (min. 1): ");
        if (scanf("%d", &(mapa + i)->tropas) != 1 || (mapa + i)->tropas < 1) {
             printf("Erro: Tropas invalidas.\n");
             return NULL;
        }
    }
    
    printf("\nCadastro concluido.\n");
    return mapa;
}

// =================================================================
// FUNCAO PRINCIPAL (main)
// =================================================================

int main() {
    // Requisito: usar srand(time(NULL))
    srand(time(NULL));
    
    struct Territorio* mapa = NULL;
    struct Jogador* jogadores = (struct Jogador*)malloc(MAX_JOGADORES * sizeof(struct Jogador));
    
    int num_territorios;
    int id_ataque, id_defesa;
    char continuar_ataque = 'S';
    int jogador_vencedor = 0;

    if (jogadores == NULL) {
        printf("\nERRO: Falha na alocacao de memoria para jogadores.\n");
        return 1;
    }

    printf("=============================================\n");
    printf("     WAR - Nivel 3: Sistema de Missoes (3)\n");
    printf("=============================================\n");

    // Requisito: A missao deve ser armazenada dinamicamente utilizando malloc.
    char* missao_temp_ptr_j1 = (char*)malloc(TAMANHO_MISSAO * sizeof(char));
    char* missao_temp_ptr_j2 = (char*)malloc(TAMANHO_MISSAO * sizeof(char));

    if (missao_temp_ptr_j1 == NULL || missao_temp_ptr_j2 == NULL) {
        printf("\nERRO: Falha na alocacao temporaria da missao.\n");
        free(missao_temp_ptr_j1); free(missao_temp_ptr_j2); free(jogadores);
        return 1;
    }

    // Jogador 1
    strcpy(jogadores[0].cor, "Vermelho");
    jogadores[0].id = 1;
    atribuirMissao(missao_temp_ptr_j1, MISSOES_ESTRATEGICAS, TOTAL_MISSOES);
    jogadores[0].missao = missao_temp_ptr_j1; 

    // Jogador 2
    strcpy(jogadores[1].cor, "Azul");
    jogadores[1].id = 2;
    atribuirMissao(missao_temp_ptr_j2, MISSOES_ESTRATEGICAS, TOTAL_MISSOES);
    jogadores[1].missao = missao_temp_ptr_j2;

    // 1. Alocacao e Cadastro
    printf("   Digite a quantidade de territorios (Min. 4): ");
    if (scanf("%d", &num_territorios) != 1 || num_territorios < 4) {
        printf("Quantidade invalida. Encerrando.\n");
        liberarMemoria(NULL, jogadores); 
        return 1;
    }
    
    mapa = cadastrar_territorios(num_territorios, jogadores);

    if (mapa == NULL) {
        liberarMemoria(NULL, jogadores);
        return 1;
    }
    
    // Requisito: Exibir a missao apenas uma vez (no inicio)
    // Requisito: Missao passada por valor (a string char* e um ponteiro, mas o valor e a string em si)
    exibirMissao(jogadores[0].missao, jogadores[0].id, jogadores[0].cor);
    exibirMissao(jogadores[1].missao, jogadores[1].id, jogadores[1].cor);

    // 2. Loop de Ataque
    do {
        while (getchar() != '\n');
        
        exibir_territorios(mapa, num_territorios);
        
        printf("\n[SISTEMA DE ATAQUE]\n");
        printf("ID do ATACANTE [0 a %d]: ", num_territorios - 1);
        if (scanf("%d", &id_ataque) != 1 || id_ataque < 0 || id_ataque >= num_territorios) break;
        
        printf("ID do DEFENSOR [0 a %d]: ", num_territorios - 1);
        if (scanf("%d", &id_defesa) != 1 || id_defesa < 0 || id_defesa >= num_territorios || id_defesa == id_ataque) break;

        struct Territorio* atacante = (mapa + id_ataque);
        struct Territorio* defensor = (mapa + id_defesa);

        // Requisito: Validacao para nao atacar territorio da mesma cor
        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("\nNao e possivel atacar seu proprio territorio!\n");
        } else {
            atacar(atacante, defensor, id_ataque, id_defesa);
        }
        
        // Requisito: Verificacao condicional (silenciosa) ao final de cada turno
        for (int i = 0; i < MAX_JOGADORES; i++) {
            // Missao passada por referencia para verificacao
            if (verificarMissao(jogadores[i].missao, mapa, num_territorios, jogadores[i].cor)) {
                jogador_vencedor = jogadores[i].id;
                break;
            }
        }
        
        // Declaracao do vencedor
        if (jogador_vencedor != 0) {
            printf("\n=============================================\n");
            printf("!!! JOGADOR %d (%s) CUMPRIU A MISSAO E VENCEU!!!\n", jogador_vencedor, jogadores[jogador_vencedor - 1].cor);
            printf("Missao: %s\n", jogadores[jogador_vencedor - 1].missao);
            printf("=============================================\n");
            continuar_ataque = 'N'; 
            break;
        }

        printf("\nDeseja realizar outro ataque? (S/N): ");
        while (getchar() != '\n');
        scanf("%c", &continuar_ataque);
        
    } while (continuar_ataque == 'S' || continuar_ataque == 's');


    // 3. Limpeza
    liberarMemoria(mapa, jogadores);
    
    printf("\nProcesso finalizado com sucesso.\n");

    return 0;
}