# 🗜️ Huffman Compressor & Decompressor

Projeto desenvolvido para a disciplina de **Estrutura de Dados II** da  
:contentReference[oaicite:0]{index=0}.

## 👨‍💻 Integrantes

- Cláudio Vasconcellos
- Otavio Augusto
- Davi Nicolas

---

# 📚 Sobre o Projeto

Este projeto consiste na implementação do **Algoritmo de Huffman** utilizando a linguagem **C**, com o objetivo de realizar:

- Compressão de arquivos;
- Descompressão de arquivos;
- Manipulação de bits;
- Construção de árvores binárias;
- Uso de filas de prioridade (Min-Heap);
- Aplicação prática de estruturas de dados não-lineares.

O sistema será executado via terminal utilizando argumentos de linha de comando.

---

# 🎯 Objetivo

O trabalho tem como finalidade aplicar conceitos estudados em Estrutura de Dados II através da construção de um compactador de arquivos baseado no algoritmo de Huffman.

O programa deverá ser capaz de:

- Ler um arquivo original;
- Gerar uma versão compactada no formato `.huff`;
- Reconstruir perfeitamente o arquivo original a partir do arquivo compactado.

---

# ⚙️ Funcionalidades Planejadas

## Compressão

- Leitura do arquivo byte a byte;
- Cálculo da frequência de ocorrência dos bytes;
- Construção da árvore de Huffman;
- Geração dos códigos binários;
- Criação do arquivo compactado `.huff`.

## Descompressão

- Leitura do cabeçalho do arquivo `.huff`;
- Reconstrução da árvore de Huffman;
- Decodificação dos bits;
- Restauração do arquivo original.

---

# 🧠 Estruturas de Dados Utilizadas

O projeto envolve o uso das seguintes estruturas:

- Árvores Binárias;
- Filas de Prioridade (Min-Heap);
- Tabelas Hash;
- Manipulação de Bits;
- Arquivos Binários.

---

# 📁 Estrutura do Arquivo `.huff`

O arquivo compactado seguirá o seguinte formato:

| Campo | Descrição |
|---|---|
| Contagem | Quantidade de bytes únicos |
| Tabela | Frequência dos bytes |
| Bits Lixo | Bits extras do último byte |
| Payload | Dados compactados |

---

# 💻 Compilação

Exemplo de compilação utilizando GCC:

```bash
gcc *.c -o huffman
```

---

# 🚀 Execução

## Compactar um arquivo

```bash
./huffman -c arquivo.txt compactado.huff
```

## Descompactar um arquivo

```bash
./huffman -d compactado.huff arquivo_restaurado.txt
```

---

# 🧪 Testes

Os testes do projeto incluirão:

- Arquivos de texto grandes;
- Arquivos pequenos com alta repetição;
- Arquivos binários já compactados.

---

# 📊 Resultados

> Esta seção será atualizada futuramente com:
>
> - Taxa de compressão;
> - Comparação entre tamanhos;
> - Tempo de execução;
> - Exemplos de testes realizados.

---

# 📌 Status do Projeto

🚧 Em desenvolvimento.

---

# 📖 Disciplina

**Estrutura de Dados II**  
:contentReference[oaicite:1]{index=1}

---

# 📄 Licença

Projeto acadêmico desenvolvido exclusivamente para fins educacionais.