#include <stdlib.h>
#include <stdio.h>

#define TAM 256

void inicia_tabela_com_zero(unsigned int tab[]){

    for (int i = 0; i < TAM; i++)
    {
        tab[i] = 0;
    }

}


int main(){

    FILE * f = fopen("../data/entrada.txt","rb"); // abre o txt pra leitura binária

    if (f == NULL)
    {
        printf("Banco de dados não existe!");
        return 1;
    }
    

    unsigned int vet[TAM];  //vetor em que será armazenada a tabela, visto que a tabela ASCII tem 256 elementos (contaremos a quantidade em cada casa de seu valor)

    inicia_tabela_com_zero(vet);
    
    unsigned char byte;

    while (fread(&byte,1,1,f)) //Lê a tabela Byte por Byte
    {
        vet[byte] ++;
    }
    
    fclose(f);

   
    return 0;



}