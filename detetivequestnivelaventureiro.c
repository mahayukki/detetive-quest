#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================
   ESTRUTURAS DE DADOS
   ========================= */

// Estrutura que representa um cômodo da mansão (árvore binária)
typedef struct Sala {
    char nome[50];
    char pista[100];              // Pista associada ao cômodo (pode estar vazia)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/* =========================
   FUNÇÕES DE CRIAÇÃO
   ========================= */

// cria dinamicamente um cômodo com ou sem pista
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);

    if (pista != NULL)
        strcpy(novaSala->pista, pista);
    else
        novaSala->pista[0] = '\0'; // sem pista

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// cria um novo nó de pista (BST)
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));

    if (novo == NULL) {
        printf("Erro ao alocar memória para pista.\n");
        exit(1);
    }

    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

/* =========================
   BST DE PISTAS
   ========================= */

// insere uma pista na árvore BST
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL)
        return criarPistaNode(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

// imprime as pistas em ordem alfabética (in-order)
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/* =========================
   EXPLORAÇÃO DA MANSÃO
   ========================= */

// controla a navegação e coleta de pistas
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\n📍 Você está em: %s\n", atual->nome);

        // coleta a pista, se existir
        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);

            // evita coletar a mesma pista novamente
            atual->pista[0] = '\0';
        }

        printf("\nCaminhos disponíveis:\n");
        if (atual->esquerda != NULL)
            printf(" (e) Esquerda\n");
        if (atual->direita != NULL)
            printf(" (d) Direita\n");
        printf(" (s) Sair da exploração\n");

        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        }
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        }
        else if (opcao == 's') {
            printf("\nExploração encerrada.\n");
            break;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

/* =========================
   FUNÇÃO PRINCIPAL
   ========================= */

int main() {
    // ===== criação do mapa da mansão =====
    Sala *hall = criarSala("Hall de Entrada", "Pegadas recentes no chão");
    Sala *salaEstar = criarSala("Sala de Estar", "Quadro torto escondendo um cofre");
    Sala *cozinha = criarSala("Cozinha", "Faca com manchas suspeitas");
    Sala *biblioteca = criarSala("Biblioteca", "Livro com páginas arrancadas");
    Sala *jardim = criarSala("Jardim", "Terra remexida recentemente");
    Sala *quarto = criarSala("Quarto", "Relógio parado às 22h");
    Sala *banheiro = criarSala("Banheiro", NULL);

    // ===== montagem da árvore binária =====
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = quarto;
    cozinha->direita = banheiro;

    // ===== árvore BST de pistas =====
    PistaNode *arvorePistas = NULL;

    // ===== início da exploração =====
    printf("🕵️ Bem-vindo à Mansão Misteriosa!\n");
    explorarSalasComPistas(hall, &arvorePistas);

    // ===== exibição final das pistas =====
    printf("\n📂 Pistas coletadas (ordem alfabética):\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    return 0;
}