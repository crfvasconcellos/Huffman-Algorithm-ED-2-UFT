#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct _node {
    unsigned char caracter;
    int frequencia;
    struct _node *left, *right, *next;
} Node;

typedef struct _list {
    Node *begin;
    int tam;
} List;

Node *Node_create(unsigned char caract, int freq);

List *List_create();

void List_insert_sorted(List *list, Node *node);

Node *List_remove_first(List *list);

Node *build_huffman_tree(List *list);

void generate_codes(Node *root, char **codes, char *buffer, int depth);

void free_tree(Node *root);

void print_list(List *list);

#endif
