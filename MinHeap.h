#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct _node{
    unsigned char caracter;
    int frequencia;
    struct _node * left,*right,*next;
}Node;


typedef struct _list{
    Node * begin;
    int tam;
}List;





#endif