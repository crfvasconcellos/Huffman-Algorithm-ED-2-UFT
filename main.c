/*
 * ============================================================
 *  main.c — Huffman Compressor & Decompressor (CLI)
 * ============================================================
 *  Projeto: Huffman Compressor & Decompressor
 *  Disciplina: Estrutura de Dados II — UFT
 *  Integrantes: Claudio Vasconcellos, Otavio Augusto, Davi Nicolas
 * ============================================================
 *
 *  Compilação:
 *    gcc main.c MinHeap.c -o huffman
 *
 *  Uso:
 *    ./huffman -c <entrada> <saida.huff>     (compressão)
 *    ./huffman -d <entrada.huff> <saida>     (descompressão)
 *
 * ============================================================
 *
 *  Formato do arquivo .huff:
 *  ┌──────────────┬───────────────────────────────────────────┐
 *  │ Campo        │ Descrição                                 │
 *  ├──────────────┼───────────────────────────────────────────┤
 *  │ Contagem     │ 4 bytes (int): qtde de símbolos únicos    │
 *  │ Tabela       │ N × [Byte(1) + Freq(4)] = N × 5 bytes    │
 *  │ Bits Lixo    │ 1 byte: bits não usados no último byte    │
 *  │ Payload      │ Dados codificados em bits                 │
 *  └──────────────┴───────────────────────────────────────────┘
 *
 * ============================================================ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MinHeap.h"

#define TAM 256

/* ============================================================
 *  COMPRESSOR — Codificador de Huffman
 * ============================================================
 *
 *  Etapas:
 *    1. Ler o arquivo byte a byte e contar as frequências
 *    2. Montar a fila de prioridade (lista ordenada)
 *    3. Construir a árvore de Huffman
 *    4. Gerar os códigos binários (Tabela Hash)
 *    5. Gravar o arquivo .huff (cabeçalho + payload)
 *
 * ============================================================ */
void comprimir(const char *arquivo_entrada, const char *arquivo_saida) {

    printf("============================================\n");
    printf("     HUFFMAN - COMPRESSOR\n");
    printf("============================================\n");
    printf("  Entrada: %s\n", arquivo_entrada);
    printf("  Saida:   %s\n", arquivo_saida);
    printf("============================================\n\n");

    /* ---------------------------------------------------------
     *  PASSO 1: Contagem de frequências
     * ---------------------------------------------------------
     *  Abre o arquivo em modo binário ("rb") e lê byte a byte.
     *  O vetor frequencias[256] armazena quantas vezes cada
     *  valor de byte (0 a 255) aparece no arquivo.
     * --------------------------------------------------------- */
    FILE *entrada = fopen(arquivo_entrada, "rb");
    if (!entrada) {
        printf("ERRO: Nao foi possivel abrir '%s'\n", arquivo_entrada);
        return;
    }

    int frequencias[TAM];
    /* Inicializa a tabela de frequências com zero */
    for (int i = 0; i < TAM; i++) {
        frequencias[i] = 0;
    }

    int byte_lido;
    long tamanho_original = 0;

    /* Lê byte a byte usando fgetc (retorna int para diferenciar de EOF) */
    while ((byte_lido = fgetc(entrada)) != EOF) {
        frequencias[(unsigned char)byte_lido]++;
        tamanho_original++;
    }

    fclose(entrada);

    /* Caso especial: arquivo vazio */
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

    /* ---------------------------------------------------------
     *  PASSO 2: Montar a fila de prioridade (lista ordenada)
     * ---------------------------------------------------------
     *  Para cada byte que apareceu pelo menos uma vez, cria um
     *  nó (Node) e insere na lista de forma ordenada por
     *  frequência crescente. Isso simula um Min-Heap.
     * --------------------------------------------------------- */
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

    /* ---------------------------------------------------------
     *  PASSO 3: Construir a árvore de Huffman
     * ---------------------------------------------------------
     *  Remove os dois nós de menor frequência, cria um pai com
     *  a soma das frequências, e reinsere. Repete até sobrar
     *  apenas a raiz.
     * --------------------------------------------------------- */
    Node *raiz = build_huffman_tree(lista);

    printf("[3/5] Arvore de Huffman construida.\n\n");

    /* ---------------------------------------------------------
     *  PASSO 4: Gerar os códigos binários (Tabela Hash)
     * ---------------------------------------------------------
     *  O array codigos[256] funciona como uma TABELA HASH com
     *  função hash h(k) = k (hash perfeito / endereçamento
     *  direto). Cada posição codigos[byte] armazena a string
     *  do código binário daquele byte.
     *
     *  Acesso em O(1), sem colisões.
     * --------------------------------------------------------- */
    char *codigos[TAM];
    for (int i = 0; i < TAM; i++) {
        codigos[i] = NULL;
    }

    char buffer_codigo[TAM];

    /* Caso especial: apenas 1 símbolo único (árvore com 1 folha) */
    if (raiz->left == NULL && raiz->right == NULL) {
        codigos[raiz->caracter] = strdup("0");
    } else {
        generate_codes(raiz, codigos, buffer_codigo, 0);
    }

    printf("[4/5] Codigos gerados (Tabela Hash preenchida).\n\n");

    /* Exibir tabela de códigos */
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

    /* ---------------------------------------------------------
     *  PASSO 5: Gravar o arquivo .huff
     * ---------------------------------------------------------
     *  Formato:
     *    [Contagem: 4 bytes]
     *    [Tabela: num_unicos × (1 byte + 4 bytes)]
     *    [Bits Lixo: 1 byte]
     *    [Payload: dados codificados]
     * --------------------------------------------------------- */
    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
        printf("ERRO: Nao foi possivel criar '%s'\n", arquivo_saida);
        free_tree(raiz);
        return;
    }

    /* CAMPO 1: Contagem — número de símbolos únicos (4 bytes) */
    fwrite(&num_unicos, sizeof(int), 1, saida);

    /* CAMPO 2: Tabela — pares [Byte (1 byte) + Frequência (4 bytes)] */
    for (int i = 0; i < TAM; i++) {
        if (frequencias[i] > 0) {
            unsigned char b = (unsigned char)i;
            fwrite(&b, sizeof(unsigned char), 1, saida);
            fwrite(&frequencias[i], sizeof(int), 1, saida);
        }
    }

    /* CAMPO 3: Bits lixo — placeholder (será atualizado no final) */
    long pos_lixo = ftell(saida);
    unsigned char lixo = 0;
    fwrite(&lixo, sizeof(unsigned char), 1, saida);

    /* CAMPO 4: Payload — codificação bit a bit
     *
     *  Para cada byte do arquivo original:
     *    1. Busca o código na Tabela Hash: codigos[byte]
     *    2. Para cada bit do código ('0' ou '1'):
     *       - Seta o bit correspondente no byte_atual
     *       - Quando byte_atual tem 8 bits, grava e reseta
     *    3. No final, se sobram bits, grava o último byte parcial
     *       e calcula quantos bits são "lixo" (não usados).
     */
    entrada = fopen(arquivo_entrada, "rb");
    unsigned char byte_atual = 0;  /* Byte sendo montado bit a bit */
    int bits_no_byte = 0;          /* Quantos bits já foram escritos */

    while ((byte_lido = fgetc(entrada)) != EOF) {
        char *codigo = codigos[(unsigned char)byte_lido];

        for (int i = 0; codigo[i] != '\0'; i++) {
            /* Se o bit é '1', seta o bit correspondente (MSB primeiro) */
            if (codigo[i] == '1')
                byte_atual |= (1 << (7 - bits_no_byte));

            bits_no_byte++;

            /* Quando completamos 8 bits, gravamos o byte */
            if (bits_no_byte == 8) {
                fwrite(&byte_atual, 1, 1, saida);
                byte_atual = 0;
                bits_no_byte = 0;
            }
        }
    }

    /* Se sobram bits no último byte, gravamos e calculamos o lixo */
    if (bits_no_byte > 0) {
        fwrite(&byte_atual, 1, 1, saida);
        lixo = 8 - bits_no_byte;
    }

    fclose(entrada);

    /* Volta ao campo de lixo e atualiza com o valor correto */
    fseek(saida, pos_lixo, SEEK_SET);
    fwrite(&lixo, sizeof(unsigned char), 1, saida);

    /* Obtém o tamanho final do arquivo compactado */
    fseek(saida, 0, SEEK_END);
    long tamanho_compactado = ftell(saida);

    fclose(saida);

    /* ---------------------------------------------------------
     *  Estatísticas finais
     * --------------------------------------------------------- */
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

    /* Libera memória */
    for (int i = 0; i < TAM; i++)
        if (codigos[i]) free(codigos[i]);

    free_tree(raiz);
    free(lista);
}

/* ============================================================
 *  DESCOMPRESSOR — Decodificador de Huffman
 * ============================================================
 *
 *  Etapas:
 *    1. Ler o cabeçalho (contagem + tabela de frequências)
 *    2. Ler o byte de bits lixo
 *    3. Reconstruir a árvore de Huffman (mesmas frequências)
 *    4. Decodificar o payload bit a bit, percorrendo a árvore
 *
 * ============================================================ */
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

    /* ---------------------------------------------------------
     *  PASSO 1: Ler o cabeçalho
     * --------------------------------------------------------- */

    /* CAMPO 1: Contagem de símbolos únicos */
    int num_unicos;
    if (fread(&num_unicos, sizeof(int), 1, entrada) != 1) {
        printf("ERRO: Arquivo .huff corrompido (contagem).\n");
        fclose(entrada);
        return;
    }

    /* Caso especial: arquivo compactado vazio */
    if (num_unicos == 0) {
        printf("Arquivo compactado esta vazio.\n");
        FILE *saida = fopen(arquivo_saida, "wb");
        if (saida) fclose(saida);
        fclose(entrada);
        printf("Arquivo restaurado (vazio) criado.\n");
        return;
    }

    printf("[1/4] Contagem lida: %d simbolos unicos\n", num_unicos);

    /* CAMPO 2: Tabela de frequências */
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

    /* CAMPO 3: Bits lixo */
    unsigned char lixo;
    fread(&lixo, sizeof(unsigned char), 1, entrada);
    printf("       Bits de lixo: %d\n\n", lixo);

    /* ---------------------------------------------------------
     *  PASSO 2: Reconstruir a árvore de Huffman
     * ---------------------------------------------------------
     *  Usamos as mesmas frequências lidas do cabeçalho para
     *  reconstruir a EXATA MESMA árvore de Huffman que foi
     *  usada na compressão.
     *
     *  Como a inserção na lista é determinística (ordenada por
     *  frequência e valor do byte), a árvore será idêntica.
     * --------------------------------------------------------- */
    List *lista = List_create();

    for (int i = 0; i < TAM; i++) {
        if (frequencias[i] > 0) {
            Node *no = Node_create((unsigned char)i, frequencias[i]);
            List_insert_sorted(lista, no);
        }
    }

    Node *raiz = build_huffman_tree(lista);

    printf("[3/4] Arvore de Huffman reconstruida.\n\n");

    /* ---------------------------------------------------------
     *  PASSO 3: Decodificar o payload
     * ---------------------------------------------------------
     *  Lemos o payload inteiro para a memória, depois
     *  percorremos bit a bit:
     *    - bit 0 → vai para a esquerda
     *    - bit 1 → vai para a direita
     *    - ao chegar em uma folha, grava o caractere e volta
     *      para a raiz
     *
     *  O último byte pode ter bits de lixo que devem ser
     *  descartados. Usamos a contagem total de bytes originais
     *  como critério de parada.
     * --------------------------------------------------------- */
    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
        printf("ERRO: Nao foi possivel criar '%s'\n", arquivo_saida);
        fclose(entrada);
        free_tree(raiz);
        free(lista);
        return;
    }

    /* Ler todo o payload restante para a memória */
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

    /* Caso especial: apenas 1 símbolo único (árvore com 1 folha) */
    if (raiz->left == NULL && raiz->right == NULL) {
        for (long i = 0; i < total_bytes_originais; i++) {
            fwrite(&raiz->caracter, 1, 1, saida);
            bytes_decodificados++;
        }
    } else {
        /* Decodificação normal: percorre a árvore bit a bit */
        Node *atual = raiz;

        for (long i = 0; i < tamanho_payload && bytes_decodificados < total_bytes_originais; i++) {
            /* Determina quantos bits ler deste byte */
            int bits_para_ler = 8;
            if (i == tamanho_payload - 1) {
                bits_para_ler = 8 - lixo; /* No último byte, ignorar lixo */
            }

            for (int bit = 0; bit < bits_para_ler && bytes_decodificados < total_bytes_originais; bit++) {
                /* Extrai o bit na posição 'bit' (MSB primeiro) */
                int b = (payload[i] >> (7 - bit)) & 1;

                if (b == 0)
                    atual = atual->left;
                else
                    atual = atual->right;

                /* Se chegou em uma folha, grava o caractere */
                if (atual->left == NULL && atual->right == NULL) {
                    fwrite(&atual->caracter, 1, 1, saida);
                    bytes_decodificados++;
                    atual = raiz; /* Volta para a raiz */
                }
            }
        }
    }

    fclose(saida);

    /* ---------------------------------------------------------
     *  Resultado
     * --------------------------------------------------------- */
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

    /* Libera memória */
    free(payload);
    free_tree(raiz);
    free(lista);
}

/* ============================================================
 *  INTERFACE DE LINHA DE COMANDO (CLI)
 * ============================================================
 *
 *  Uso:
 *    ./huffman -c <entrada> <saida.huff>
 *    ./huffman -d <entrada.huff> <saida>
 *
 *  Argumentos:
 *    argc    — número de argumentos (deve ser 4)
 *    argv[0] — nome do programa
 *    argv[1] — modo: "-c" (comprimir) ou "-d" (descomprimir)
 *    argv[2] — arquivo de entrada
 *    argv[3] — arquivo de saída
 *
 * ============================================================ */

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

    /* Verifica se o número de argumentos está correto */
    if (argc != 4) {
        printf("\nERRO: Numero incorreto de argumentos.\n\n");
        mostrar_uso(argv[0]);
        return 1;
    }

    const char *modo            = argv[1]; /* "-c" ou "-d" */
    const char *arquivo_entrada = argv[2]; /* Arquivo de entrada */
    const char *arquivo_saida   = argv[3]; /* Arquivo de saída */

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