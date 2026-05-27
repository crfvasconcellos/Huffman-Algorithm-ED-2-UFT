#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MinHeap.h"

#define TAM 256

void comprimir(const char *arquivo_entrada, const char *arquivo_saida) {

    printf("============================================\n");
    printf("     HUFFMAN - COMPRESSOR\n");
    printf("============================================\n");
    printf("  Entrada: %s\n", arquivo_entrada);
    printf("  Saida:   %s\n", arquivo_saida);
    printf("============================================\n\n");

    FILE *entrada = fopen(arquivo_entrada, "rb");
    if (!entrada) {
        printf("ERRO: Nao foi possivel abrir '%s'\n", arquivo_entrada);
        return;
    }

    int frequencias[TAM];

    for (int i = 0; i < TAM; i++) {
        frequencias[i] = 0;
    }

    int byte_lido;
    long tamanho_original = 0;

    while ((byte_lido = fgetc(entrada)) != EOF) {
        frequencias[(unsigned char)byte_lido]++;
        tamanho_original++;
    }

    fclose(entrada);

    if (tamanho_original == 0) {
        printf("Arquivo vazio! Criando .huff vazio.\n");
        FILE *saida = fopen(arquivo_saida, "wb");
        if (!saida) {
            printf("ERRO: Nao foi possivel criar '%s'\n", arquivo_saida);
            return;
        }
        int count = 0;
        fwrite(&count, sizeof(int), 1, saida);
        unsigned char lixo = 0;
        fwrite(&lixo, 1, 1, saida);
        fclose(saida);
        printf("Concluido.\n");
        return;
    }

    printf("[1/5] Frequencias calculadas.\n");
    printf("       Tamanho original: %ld bytes\n\n", tamanho_original);

    List *lista = List_create();
    int num_unicos = 0;

    for (int i = 0; i < TAM; i++) {
        if (frequencias[i] > 0) {
            Node *no = Node_create((unsigned char)i, frequencias[i]);
            List_insert_sorted(lista, no);
            num_unicos++;
        }
    }

    printf("[2/5] Fila de prioridade criada.\n");
    printf("       Simbolos unicos: %d\n\n", num_unicos);

    Node *raiz = build_huffman_tree(lista);

    printf("[3/5] Arvore de Huffman construida.\n\n");

    char *codigos[TAM];
    for (int i = 0; i < TAM; i++) {
        codigos[i] = NULL;
    }

    char buffer_codigo[TAM];

    if (raiz->left == NULL && raiz->right == NULL) {
        codigos[raiz->caracter] = strdup("0");
    } else {
        generate_codes(raiz, codigos, buffer_codigo, 0);
    }

    printf("[4/5] Codigos gerados (Tabela Hash preenchida).\n\n");

    printf("  +---------+----------+----------------------+\n");
    printf("  |  Byte   |   Freq   |  Codigo Huffman      |\n");
    printf("  +---------+----------+----------------------+\n");
    for (int i = 0; i < TAM; i++) {
        if (codigos[i] != NULL) {
            if (i >= 32 && i <= 126)
                printf("  |  '%c'    | %-8d | %-20s |\n",
                       i, frequencias[i], codigos[i]);
            else
                printf("  |  0x%02X   | %-8d | %-20s |\n",
                       i, frequencias[i], codigos[i]);
        }
    }
    printf("  +---------+----------+----------------------+\n\n");

    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
        printf("ERRO: Nao foi possivel criar '%s'\n", arquivo_saida);
        free_tree(raiz);
        return;
    }

    fwrite(&num_unicos, sizeof(int), 1, saida);

    for (int i = 0; i < TAM; i++) {
        if (frequencias[i] > 0) {
            unsigned char b = (unsigned char)i;
            fwrite(&b, sizeof(unsigned char), 1, saida);
            fwrite(&frequencias[i], sizeof(int), 1, saida);
        }
    }

    long pos_lixo = ftell(saida);
    unsigned char lixo = 0;
    fwrite(&lixo, sizeof(unsigned char), 1, saida);

    entrada = fopen(arquivo_entrada, "rb");
    unsigned char byte_atual = 0;  
    int bits_no_byte = 0;          

    while ((byte_lido = fgetc(entrada)) != EOF) {
        char *codigo = codigos[(unsigned char)byte_lido];

        for (int i = 0; codigo[i] != '\0'; i++) {

            if (codigo[i] == '1')
                byte_atual |= (1 << (7 - bits_no_byte));

            bits_no_byte++;

            if (bits_no_byte == 8) {
                fwrite(&byte_atual, 1, 1, saida);
                byte_atual = 0;
                bits_no_byte = 0;
            }
        }
    }

    if (bits_no_byte > 0) {
        fwrite(&byte_atual, 1, 1, saida);
        lixo = 8 - bits_no_byte;
    }

    fclose(entrada);

    fseek(saida, pos_lixo, SEEK_SET);
    fwrite(&lixo, sizeof(unsigned char), 1, saida);

    fseek(saida, 0, SEEK_END);
    long tamanho_compactado = ftell(saida);

    fclose(saida);

    double taxa = (1.0 - (double)tamanho_compactado / tamanho_original) * 100.0;

    printf("[5/5] Compressao concluida!\n\n");
    printf("  +-------------------------------+\n");
    printf("  |       RESULTADO               |\n");
    printf("  +-------------------------------+\n");
    printf("  | Original:    %8ld bytes   |\n", tamanho_original);
    printf("  | Compactado:  %8ld bytes   |\n", tamanho_compactado);
    printf("  | Compressao:  %7.2f%%        |\n", taxa);
    printf("  | Bits lixo:   %8d         |\n", lixo);
    printf("  +-------------------------------+\n");
    printf("============================================\n");

    for (int i = 0; i < TAM; i++)
        if (codigos[i]) free(codigos[i]);

    free_tree(raiz);
    free(lista);
}

void descomprimir(const char *arquivo_huff, const char *arquivo_saida) {

    printf("============================================\n");
    printf("     HUFFMAN - DESCOMPRESSOR\n");
    printf("============================================\n");
    printf("  Entrada: %s\n", arquivo_huff);
    printf("  Saida:   %s\n", arquivo_saida);
    printf("============================================\n\n");

    FILE *entrada = fopen(arquivo_huff, "rb");
    if (!entrada) {
        printf("ERRO: Nao foi possivel abrir '%s'\n", arquivo_huff);
        return;
    }

    int num_unicos;
    if (fread(&num_unicos, sizeof(int), 1, entrada) != 1) {
        printf("ERRO: Arquivo .huff corrompido (contagem).\n");
        fclose(entrada);
        return;
    }

    if (num_unicos == 0) {
        printf("Arquivo compactado esta vazio.\n");
        FILE *saida = fopen(arquivo_saida, "wb");
        if (saida) fclose(saida);
        fclose(entrada);
        printf("Arquivo restaurado (vazio) criado.\n");
        return;
    }

    printf("[1/4] Contagem lida: %d simbolos unicos\n", num_unicos);

    int frequencias[TAM];
    for (int i = 0; i < TAM; i++) {
        frequencias[i] = 0;
    }

    long total_bytes_originais = 0;

    for (int i = 0; i < num_unicos; i++) {
        unsigned char b;
        int freq;
        fread(&b, sizeof(unsigned char), 1, entrada);
        fread(&freq, sizeof(int), 1, entrada);
        frequencias[b] = freq;
        total_bytes_originais += freq;
    }

    printf("[2/4] Tabela de frequencias lida.\n");
    printf("       Total de bytes originais: %ld\n", total_bytes_originais);

    unsigned char lixo;
    fread(&lixo, sizeof(unsigned char), 1, entrada);
    printf("       Bits de lixo: %d\n\n", lixo);

    List *lista = List_create();

    for (int i = 0; i < TAM; i++) {
        if (frequencias[i] > 0) {
            Node *no = Node_create((unsigned char)i, frequencias[i]);
            List_insert_sorted(lista, no);
        }
    }

    Node *raiz = build_huffman_tree(lista);

    printf("[3/4] Arvore de Huffman reconstruida.\n\n");

    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
        printf("ERRO: Nao foi possivel criar '%s'\n", arquivo_saida);
        fclose(entrada);
        free_tree(raiz);
        free(lista);
        return;
    }

    long pos_payload = ftell(entrada);
    fseek(entrada, 0, SEEK_END);
    long tamanho_payload = ftell(entrada) - pos_payload;
    fseek(entrada, pos_payload, SEEK_SET);

    if (tamanho_payload <= 0) {
        printf("ERRO: Nenhum dado de payload encontrado.\n");
        fclose(entrada);
        fclose(saida);
        free_tree(raiz);
        free(lista);
        return;
    }

    unsigned char *payload = (unsigned char *)malloc(tamanho_payload);
    if (!payload) {
        printf("ERRO: Falha ao alocar memoria para o payload.\n");
        fclose(entrada);
        fclose(saida);
        free_tree(raiz);
        free(lista);
        return;
    }

    fread(payload, 1, tamanho_payload, entrada);
    fclose(entrada);

    long bytes_decodificados = 0;

    if (raiz->left == NULL && raiz->right == NULL) {
        for (long i = 0; i < total_bytes_originais; i++) {
            fwrite(&raiz->caracter, 1, 1, saida);
            bytes_decodificados++;
        }
    } else {

        Node *atual = raiz;

        for (long i = 0; i < tamanho_payload && bytes_decodificados < total_bytes_originais; i++) {

            int bits_para_ler = 8;
            if (i == tamanho_payload - 1) {
                bits_para_ler = 8 - lixo; 
            }

            for (int bit = 0; bit < bits_para_ler && bytes_decodificados < total_bytes_originais; bit++) {

                int b = (payload[i] >> (7 - bit)) & 1;

                if (b == 0)
                    atual = atual->left;
                else
                    atual = atual->right;

                if (atual->left == NULL && atual->right == NULL) {
                    fwrite(&atual->caracter, 1, 1, saida);
                    bytes_decodificados++;
                    atual = raiz; 
                }
            }
        }
    }

    fclose(saida);

    printf("[4/4] Decodificacao concluida!\n\n");
    printf("  +-------------------------------+\n");
    printf("  |       RESULTADO               |\n");
    printf("  +-------------------------------+\n");
    printf("  | Bytes decodificados: %8ld |\n", bytes_decodificados);
    printf("  | Tamanho esperado:    %8ld |\n", total_bytes_originais);
    printf("  +-------------------------------+\n");

    if (bytes_decodificados == total_bytes_originais)
        printf("  | Status: OK - Sucesso!        |\n");
    else
        printf("  | Status: ERRO - Divergencia!  |\n");

    printf("  +-------------------------------+\n");
    printf("============================================\n");

    free(payload);
    free_tree(raiz);
    free(lista);
}

void mostrar_uso(const char *nome_programa) {
    printf("============================================\n");
    printf("     HUFFMAN - Compressor/Descompressor\n");
    printf("============================================\n");
    printf("  Disciplina: Estrutura de Dados II - UFT\n");
    printf("============================================\n\n");
    printf("  Uso:\n");
    printf("    %s -c <entrada> <saida.huff>\n", nome_programa);
    printf("    %s -d <entrada.huff> <saida>\n\n", nome_programa);
    printf("  Opcoes:\n");
    printf("    -c   Comprimir (codificar)\n");
    printf("    -d   Descomprimir (decodificar)\n");
    printf("============================================\n");
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("\nERRO: Numero incorreto de argumentos.\n\n");
        mostrar_uso(argv[0]);
        return 1;
    }

    const char *modo            = argv[1]; 
    const char *arquivo_entrada = argv[2]; 
    const char *arquivo_saida   = argv[3]; 

    if (strcmp(modo, "-c") == 0) {
        comprimir(arquivo_entrada, arquivo_saida);

    } else if (strcmp(modo, "-d") == 0) {
        descomprimir(arquivo_entrada, arquivo_saida);

    } else {
        printf("\nERRO: Modo '%s' desconhecido.\n\n", modo);
        mostrar_uso(argv[0]);
        return 1;
    }

    return 0;
}
