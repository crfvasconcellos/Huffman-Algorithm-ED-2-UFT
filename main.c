#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM 256


void comprimir(const char *arquivo_entrada, const char *arquivo_saida) {
    printf("Comprimindo '%s' -> '%s'\n", arquivo_entrada, arquivo_saida);
}

void descomprimir(const char *arquivo_huff, const char *arquivo_saida) {
    printf("Descomprimindo '%s' -> '%s'\n", arquivo_huff, arquivo_saida);
}

void mostrar_uso(const char *nome_programa) {
    printf("Uso:\n");
    printf(" Comprimir:    %s -c <entrada> <saida.huff>\n", nome_programa);
    printf(" Descomprimir: %s -d <entrada.huff> <saida>\n", nome_programa);
}

void inicia_tabela_com_zero(unsigned int tab[]){

    for (int i = 0; i < TAM; i++)
    {
        tab[i] = 0;
    }

}


int main(){


    /*if(argc!=4) {               // parte do davi
        printf("ERRO! numero incorreto de argumentos.\n");
        mostrar_uso(argv[0]);
        return 1;
    }

    const char *modo           = argv[1]; //modos
    const char *arquivo_entrada= argv[2]; //arquivo de entrada
    const char *arquivo_saida  = argv[3]; //arquivo de saída

    if(strcmp(modo,"-c") == 0) {
        comprimir(arquivo_entrada, arquivo_saida);

    }else if(strcmp(modo,"-d") == 0) {
        descomprimir(arquivo_entrada, arquivo_saida);

    }else {
        printf("Erro: modo '%s' desconhecido.\n", modo);
        mostrar_uso(argv[0]);
        return 1;
    }
    return 0;*/

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
