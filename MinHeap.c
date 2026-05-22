#include "MinHeap.h"
#include <stdio.h>
#include <stdlib.h>


Node * Node_create(char caract,int freq){
    Node * no = malloc(sizeof(Node));

    no->caracter = caract;
    no->frequencia = freq;
    no->left = NULL;
    no->right = NULL;
    no->next= NULL;

    return no;

}

List * List_create(){
    List * list = malloc(sizeof(List));
    list->begin = NULL;
    list->tam = 0;

    return list;

}



