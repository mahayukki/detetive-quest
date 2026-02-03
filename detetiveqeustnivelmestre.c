#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

/* =========================
   ESTRUTURAS DE DADOS
   ========================= */

// Estrutura da árvore binária da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];      // pista associada à sala
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Estrutura da BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// Estrutura da tabela hash (pista → suspeito)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

/* =========================
   TABELA HASH
   ========================= */

HashNode* tabelaHash[TAM_HASH];

// Função hash simples
int funcaoHash(const char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_HASH;
}

// insere associação pista/suspeito na tabela hash
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// consulta o suspeito associado a uma pista
char* encontrarSuspeito(const char *pista) {
    int indice = funcaoHash(pista);
    HashNode *aux = tabelaHash[indice];

    while (aux != NULL) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }
    return NULL;
}

/* =========================
   ÁRVORE BST DE PISTAS
   ========================= */

// insere pista na BST
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

// exibe pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

/* =========================
   ÁRVORE DE SALAS
   ========================= */

// cria dinamicamente um cômodo
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = nova->dir = NULL;
    return nova;
}

/* =========================
   EXPLORAÇÃO
   ========================= */

// navega pela mansão e coleta pistas
void explorarSalas(Sala *atual, PistaNode **bstPistas) {
    char opcao;

    while (atual != NULL) {
        printf("\n📍 Sala atual: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: %s\n", atual->pista);
            *bstPistas = inserirPista(*bstPistas, atual->pista);
            atual->pista[0] = '\0'; // evita coletar novamente
        }

        printf("\nOpções:\n");
        if (atual->esq) printf(" (e) Esquerda\n");
        if (atual->dir) printf(" (d) Direita\n");
        printf(" (s) Sair\n");

        printf("Escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esq)
            atual = atual->esq;
        else if (opcao == 'd' && atual->dir)
            atual = atual->dir;
        else if (opcao == 's')
            break;
        else
            printf("Opção inválida!\n");
    }
}

/* =========================
   JULGAMENTO FINAL
   ========================= */

// conta quantas pistas apontam para um suspeito
int contarPistasSuspeito(PistaNode *raiz, const char *suspeito) {
    if (raiz == NULL)
        return 0;

    int count = 0;
    char *resp = encontrarSuspeito(raiz->pista);

    if (resp != NULL && strcmp(resp, suspeito) == 0)
        count++;

    return count +
           contarPistasSuspeito(raiz->esq, suspeito) +
           contarPistasSuspeito(raiz->dir, suspeito);
}

// fase final de acusação
void verificarSuspeitoFinal(PistaNode *bstPistas) {
    char acusado[50];
    int total;

    printf("\n🧑‍⚖️ Fase de Julgamento Final\n");
    printf("Digite o nome do suspeito: ");
    scanf(" %[^\n]", acusado);

    total = contarPistasSuspeito(bstPistas, acusado);

    printf("\nPistas contra %s: %d\n", acusado, total);

    if (total >= 2)
        printf("✅ Acusação CONFIRMADA! O suspeito é culpado.\n");
    else
        printf("❌ Provas insuficientes. O suspeito é inocente.\n");
}

/* =========================
   MAIN
   ========================= */

int main() {
    // inicializa hash
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;

    // associa pistas a suspeitos
    inserirNaHash("Faca ensanguentada", "Sr. Carlos");
    inserirNaHash("Pegadas no jardim", "Sr. Carlos");
    inserirNaHash("Relógio quebrado", "Dona Marta");
    inserirNaHash("Carta rasgada", "Dona Marta");
    inserirNaHash("Luvas pretas", "Sr. João");

    // cria salas
    Sala *hall = criarSala("Hall", "Pegadas no jardim");
    Sala *sala = criarSala("Sala de Estar", "Carta rasgada");
    Sala *cozinha = criarSala("Cozinha", "Faca ensanguentada");
    Sala *quarto = criarSala("Quarto", "Relógio quebrado");
    Sala *jardim = criarSala("Jardim", "");
    Sala *banheiro = criarSala("Banheiro", "Luvas pretas");

    // monta árvore
    hall->esq = sala;
    hall->dir = cozinha;
    sala->esq = quarto;
    sala->dir = jardim;
    cozinha->dir = banheiro;

    // BST de pistas
    PistaNode *bstPistas = NULL;

    printf("🕵️ Bem-vindo à Mansão do Mistério!\n");
    explorarSalas(hall, &bstPistas);

    printf("\n📂 Pistas coletadas:\n");
    exibirPistas(bstPistas);

    verificarSuspeitoFinal(bstPistas);

    return 0;
}