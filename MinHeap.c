#include "MinHeap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void List_insert_sorted(List *list, Node *node) {
    if (list == NULL || node == NULL) return;

    if (list->begin == NULL ||
        node->frequencia < list->begin->frequencia ||
        (node->frequencia == list->begin->frequencia &&
         node->caracter < list->begin->caracter)) {

        node->next  = list->begin;
        list->begin = node;

    } else {

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

Node *List_remove_first(List *list) {
    if (list == NULL || list->begin == NULL)
        return NULL;

    Node *removido = list->begin;
    list->begin    = removido->next;
    removido->next = NULL;
    list->tam--;

    return removido;
}

Node *build_huffman_tree(List *list) {
    if (list == NULL || list->begin == NULL)
        return NULL;

    if (list->tam == 1)
        return List_remove_first(list);

    while (list->tam > 1) {

        Node *esquerda = List_remove_first(list);
        Node *direita  = List_remove_first(list);

        Node *pai = Node_create('*', esquerda->frequencia + direita->frequencia);

        pai->left  = esquerda;
        pai->right = direita;

        List_insert_sorted(list, pai);
    }

    return List_remove_first(list);
}

void generate_codes(Node *root, char **codes, char *buffer, int depth) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        buffer[depth] = '\0';                    
        codes[root->caracter] = strdup(buffer);  
        return;
    }

    buffer[depth] = '0';
    generate_codes(root->left, codes, buffer, depth + 1);

    buffer[depth] = '1';
    generate_codes(root->right, codes, buffer, depth + 1);
}

void free_tree(Node *root) {
    if (root == NULL) return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

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
