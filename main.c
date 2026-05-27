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

void comprimir(const char *arquivo_entrada, const char *arquivo_saida){
     printf("Comprimindo '%s' -> '%s'\n", arquivo_entrada, arquivo_saida);

    FILE *entrada = fopen(arquivo_entrada, "rb");
    if (!entrada) {
        printf("Erro! '%s' nao pode ser lido.\n", arquivo_entrada);
        return;
    }

    // Array de 256 posições — índice = valor do byte, conteúdo = frequência
    unsigned long int frequencias[256] = {0};

    int byte_lido;
    while ((byte_lido = fgetc(entrada)) != EOF) {
        frequencias[(unsigned char)byte_lido]++;
    }
    fclose(entrada);

    // Monta a lista com os bytes que apareceram pelo menos uma vez
    ListaDup *Lista = criar_Lista_Dup();
    if (!Lista) {
        printf("Erro! Memoria insuficiente.\n");
        return;
    }

    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            No *novoNo = CriarNo((unsigned char)i, frequencias[i]);
            Inserir(novoNo, Lista);
        }
    }

    printf("Simbolos unicos encontrados: %d\n", Lista->Numero_Itens);

    // --- Gravação do arquivo .huff ---
    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
        printf("Erro! '%s' nao pode ser criado.\n", arquivo_saida);
        return;
    }

    // CAMPO 1: número de símbolos únicos (4 bytes)
    fwrite(&Lista->Numero_Itens, sizeof(int), 1, saida);

    // CAMPO 2: tabela — valor do byte (1 byte) + frequência (8 bytes), por nó
    No *aux = Lista->Raiz;
    while (aux != NULL) {
        fwrite(&aux->valor,            sizeof(unsigned char),     1, saida);
        fwrite(&aux->numero_Aparicoes, sizeof(unsigned long int), 1, saida);
        aux = aux->sucessor;
    }

    // CAMPO 3: bits de lixo (placeholder — implementar com o payload)
    unsigned char lixo = 0;
    fwrite(&lixo, sizeof(unsigned char), 1, saida);

    // CAMPO 4: payload (a ser implementado com a árvore de Huffman)

    fclose(saida);
    printf("Concluido! '%s' criado.\n", arquivo_saida);
}


int main(){

    // parte do davi
    if(argc!=4) {               // parte do davi
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
    return 0;

    // fim da parte do davi

    // parte claudio

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

    // fim da parte do claudio


}
