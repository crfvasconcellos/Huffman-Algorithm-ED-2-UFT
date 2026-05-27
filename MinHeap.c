/*
 * ============================================================
 *  MinHeap.c — Implementação das Estruturas de Dados (Huffman)
 * ============================================================
 *  Projeto: Huffman Compressor & Decompressor
 *  Disciplina: Estrutura de Dados II — UFT
 *  Integrantes: Claudio Vasconcellos, Otavio Augusto, Davi Nicolas
 * ============================================================
 *
 *  Este arquivo implementa:
 *    1. Criação de nós e listas
 *    2. Lista encadeada ordenada (fila de prioridade / Min-Heap)
 *    3. Construção da árvore binária de Huffman
 *    4. Geração dos códigos binários (travessia recursiva)
 *    5. Liberação de memória
 */

#include "MinHeap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 *  1. FUNÇÕES DE CRIAÇÃO
 * ============================================================ */

/*
 * Node_create — Cria e inicializa um novo nó.
 *
 * Parâmetros:
 *   caract — o byte representado por este nó (0–255)
 *   freq   — a frequência (número de ocorrências) desse byte
 *
 * Retorno:
 *   Ponteiro para o novo nó alocado, ou NULL em caso de erro.
 *
 * Funcionamento:
 *   1. Aloca memória para um Node com malloc.
 *   2. Preenche o caractere e a frequência.
 *   3. Inicializa os ponteiros left, right e next como NULL.
 */
Node *Node_create(unsigned char caract, int freq) {
    Node *no = (Node *)malloc(sizeof(Node));
    if (no == NULL) {
        printf("ERRO: Falha ao alocar memoria para o no.\n");
        return NULL;
    }

    no->caracter   = caract;
    no->frequencia = freq;
    no->left       = NULL;
    no->right      = NULL;
    no->next       = NULL;

    return no;
}

/*
 * List_create — Cria uma lista encadeada vazia.
 *
 * Retorno:
 *   Ponteiro para a lista alocada (begin = NULL, tam = 0).
 */
List *List_create() {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) {
        printf("ERRO: Falha ao alocar memoria para a lista.\n");
        return NULL;
    }

    list->begin = NULL;
    list->tam   = 0;

    return list;
}

/* ============================================================
 *  2. FILA DE PRIORIDADE (LISTA ENCADEADA ORDENADA)
 * ============================================================
 *
 *  A lista é mantida sempre ordenada por frequência crescente.
 *  Isso garante que o primeiro elemento (begin) sempre seja
 *  o de MENOR frequência — comportamento idêntico a um Min-Heap.
 *
 *  Complexidade de inserção: O(n) no pior caso (n ≤ 256).
 *  Complexidade de remoção do mínimo: O(1).
 * ============================================================ */

/*
 * List_insert_sorted — Insere um nó na posição correta (ordenada).
 *
 * Parâmetros:
 *   list — ponteiro para a lista
 *   node — ponteiro para o nó a ser inserido
 *
 * Critério de ordenação:
 *   1º: frequência crescente
 *   2º: em caso de empate, valor do caractere crescente
 *        (garante determinismo na construção da árvore)
 *
 * Funcionamento:
 *   - Se a lista está vazia OU o nó deve ficar antes do begin,
 *     insere no início.
 *   - Caso contrário, percorre a lista até encontrar a posição
 *     correta e insere entre dois nós.
 */
void List_insert_sorted(List *list, Node *node) {
    if (list == NULL || node == NULL) return;

    /* Caso 1: lista vazia ou nó deve ficar antes do primeiro */
    if (list->begin == NULL ||
        node->frequencia < list->begin->frequencia ||
        (node->frequencia == list->begin->frequencia &&
         node->caracter < list->begin->caracter)) {

        node->next  = list->begin;
        list->begin = node;

    } else {
        /* Caso 2: percorrer até encontrar a posição correta */
        Node *aux = list->begin;

        while (aux->next != NULL &&
               (aux->next->frequencia < node->frequencia ||
                (aux->next->frequencia == node->frequencia &&
                 aux->next->caracter <= node->caracter))) {
            aux = aux->next;
        }

        node->next = aux->next;
        aux->next  = node;
    }

    list->tam++;
}

/*
 * List_remove_first — Remove e retorna o primeiro nó (menor frequência).
 *
 * Retorno:
 *   Ponteiro para o nó removido, ou NULL se a lista estiver vazia.
 *
 * Funcionamento:
 *   1. Salva o ponteiro do begin.
 *   2. Avança o begin para o próximo nó.
 *   3. Desconecta o nó removido (next = NULL).
 *   4. Decrementa o tamanho.
 */
Node *List_remove_first(List *list) {
    if (list == NULL || list->begin == NULL)
        return NULL;

    Node *removido = list->begin;
    list->begin    = removido->next;
    removido->next = NULL;
    list->tam--;

    return removido;
}

/* ============================================================
 *  3. CONSTRUÇÃO DA ÁRVORE DE HUFFMAN
 * ============================================================
 *
 *  Algoritmo:
 *    1. Enquanto a lista tiver mais de 1 nó:
 *       a) Remove os dois nós com menor frequência (os dois primeiros).
 *       b) Cria um nó pai com frequência = soma dos dois filhos.
 *       c) O filho com menor frequência vai para a esquerda.
 *       d) Insere o nó pai de volta na lista (ordenado).
 *    2. O último nó restante é a raiz da árvore.
 *
 *  O nó pai utiliza o caractere '*' (convenção para nós internos).
 * ============================================================ */

Node *build_huffman_tree(List *list) {
    if (list == NULL || list->begin == NULL)
        return NULL;

    /* Caso especial: apenas 1 símbolo único */
    if (list->tam == 1)
        return List_remove_first(list);

    while (list->tam > 1) {
        /* Remove os dois menores */
        Node *esquerda = List_remove_first(list);
        Node *direita  = List_remove_first(list);

        /* Cria o nó pai (nó interno) */
        Node *pai = Node_create('*', esquerda->frequencia + direita->frequencia);

        pai->left  = esquerda;
        pai->right = direita;

        /* Reinsere o pai na lista de forma ordenada */
        List_insert_sorted(list, pai);
    }

    /* A raiz é o único nó restante */
    return List_remove_first(list);
}

/* ============================================================
 *  4. GERAÇÃO DOS CÓDIGOS BINÁRIOS
 * ============================================================
 *
 *  Percorre a árvore de forma recursiva (pré-ordem):
 *    - Ao ir para a esquerda:  acrescenta '0' ao buffer
 *    - Ao ir para a direita:   acrescenta '1' ao buffer
 *    - Ao chegar em uma folha: copia o buffer para codes[caracter]
 *
 *  Os códigos são armazenados no array codes[256], que funciona
 *  como uma TABELA HASH com função hash h(k) = k (hash perfeito).
 *  Ou seja, codes[byte] retorna diretamente o código desse byte
 *  em tempo O(1), sem colisões.
 * ============================================================ */

void generate_codes(Node *root, char **codes, char *buffer, int depth) {
    if (root == NULL) return;

    /* Se é uma folha (sem filhos), salvamos o código */
    if (root->left == NULL && root->right == NULL) {
        buffer[depth] = '\0';                    /* Termina a string */
        codes[root->caracter] = strdup(buffer);  /* Copia para a tabela */
        return;
    }

    /* Desce para a esquerda com bit '0' */
    buffer[depth] = '0';
    generate_codes(root->left, codes, buffer, depth + 1);

    /* Desce para a direita com bit '1' */
    buffer[depth] = '1';
    generate_codes(root->right, codes, buffer, depth + 1);
}

/* ============================================================
 *  5. UTILITÁRIOS
 * ============================================================ */

/*
 * free_tree — Libera toda a memória da árvore (travessia pós-ordem).
 *
 * Funcionamento:
 *   1. Libera recursivamente a subárvore esquerda.
 *   2. Libera recursivamente a subárvore direita.
 *   3. Libera o nó atual.
 */
void free_tree(Node *root) {
    if (root == NULL) return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/*
 * print_list — Imprime os nós da lista (para fins de depuração).
 */
void print_list(List *list) {
    if (list == NULL) return;

    printf("Lista (%d nos):\n", list->tam);
    Node *aux = list->begin;
    while (aux != NULL) {
        if (aux->caracter >= 32 && aux->caracter <= 126)
            printf("  ['%c'] freq=%d\n", aux->caracter, aux->frequencia);
        else
            printf("  [0x%02X] freq=%d\n", aux->caracter, aux->frequencia);
        aux = aux->next;
    }
}
