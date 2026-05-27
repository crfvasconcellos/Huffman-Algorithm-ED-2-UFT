/*
 * ============================================================
 *  MinHeap.h — Estruturas de Dados para o Algoritmo de Huffman
 * ============================================================
 *  Projeto: Huffman Compressor & Decompressor
 *  Disciplina: Estrutura de Dados II — UFT
 *  Integrantes: Claudio Vasconcellos, Otavio Augusto, Davi Nicolas
 * ============================================================
 */

#ifndef MINHEAP_H
#define MINHEAP_H

/* ----------------------------------------------------------------
 *  Node — Nó da Árvore de Huffman / Elemento da Lista Encadeada
 * ----------------------------------------------------------------
 *  - caracter:   valor do byte (0–255)
 *  - frequencia: número de ocorrências desse byte no arquivo
 *  - left/right: ponteiros para filhos (árvore binária)
 *  - next:       ponteiro para próximo nó (lista encadeada ordenada)
 * ---------------------------------------------------------------- */
typedef struct _node {
    unsigned char caracter;
    int frequencia;
    struct _node *left, *right, *next;
} Node;

/* ----------------------------------------------------------------
 *  List — Lista Encadeada Ordenada (Fila de Prioridade / Min-Heap)
 * ----------------------------------------------------------------
 *  Funciona como fila de prioridade: o nó com menor frequência
 *  está sempre no início da lista (begin).
 *  - begin: ponteiro para o primeiro nó (menor frequência)
 *  - tam:   quantidade de nós na lista
 * ---------------------------------------------------------------- */
typedef struct _list {
    Node *begin;
    int tam;
} List;

/* =========================
 *  Funções de Criação
 * ========================= */

/* Cria um novo nó com o caractere e frequência informados.
 * Todos os ponteiros (left, right, next) são inicializados como NULL. */
Node *Node_create(unsigned char caract, int freq);

/* Cria uma lista vazia (begin = NULL, tam = 0). */
List *List_create();

/* =========================
 *  Fila de Prioridade
 * ========================= */

/* Insere um nó na lista de forma ordenada por frequência crescente.
 * Em caso de empate na frequência, ordena pelo valor do caractere. */
void List_insert_sorted(List *list, Node *node);

/* Remove e retorna o primeiro nó da lista (o de menor frequência).
 * Retorna NULL se a lista estiver vazia. */
Node *List_remove_first(List *list);

/* =========================
 *  Árvore de Huffman
 * ========================= */

/* Constrói a árvore de Huffman a partir da lista ordenada.
 * Remove os dois menores, cria um pai com freq = soma, reinsere.
 * Repete até sobrar 1 nó (a raiz da árvore).
 * Retorna a raiz. */
Node *build_huffman_tree(List *list);

/* Percorre a árvore recursivamente e gera os códigos binários.
 * - root:   nó atual na travessia
 * - codes:  array de 256 strings (Tabela Hash) onde codes[byte] = código
 * - buffer: string temporária para montar o código durante a recursão
 * - depth:  profundidade atual (número de bits no código até aqui) */
void generate_codes(Node *root, char **codes, char *buffer, int depth);

/* =========================
 *  Utilitários
 * ========================= */

/* Libera toda a memória da árvore (pós-ordem). */
void free_tree(Node *root);

/* Imprime os nós da lista (para debug). */
void print_list(List *list);

#endif