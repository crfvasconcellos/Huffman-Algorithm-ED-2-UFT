# 🗜️ Huffman Compressor & Decompressor

Projeto desenvolvido para a disciplina de **Estrutura de Dados II** da **Universidade Federal do Tocantins (UFT)**.

## 👨‍💻 Integrantes

- Cláudio Vasconcellos
- Otavio Augusto
- Davi Nicolas

---

## 📚 Sobre o Projeto

Implementação completa do **Algoritmo de Huffman** em linguagem **C**, capaz de:

- **Compactar** qualquer arquivo (texto, binário, imagem, etc.) gerando um arquivo `.huff`
- **Descompactar** o arquivo `.huff` restaurando o arquivo original **sem perda de nenhum byte**

---

## 🧠 Estruturas de Dados Utilizadas

| Estrutura | Onde é usada | Arquivo |
|---|---|---|
| **Árvore Binária** | Árvore de Huffman (codificação/decodificação) | `MinHeap.c` |
| **Fila de Prioridade (Min-Heap)** | Lista encadeada ordenada por frequência | `MinHeap.c` |
| **Tabela Hash** | Array `codigos[256]` com hash `h(k) = k` para mapear byte → código | `main.c` |

---

## 📁 Estrutura do Projeto

```
NosTres-ED2-UFT/
├── main.c          # Programa principal (CLI + compressor + descompressor)
├── MinHeap.c       # Implementação das estruturas de dados
├── MinHeap.h       # Definição das structs e protótipos
├── data/
│   └── entrada.txt # Arquivo de teste
├── output/
│   └── huffman.exe # Executável compilado
└── README.md
```

---

## 📁 Formato do Arquivo `.huff`

| Campo | Tamanho | Descrição |
|---|---|---|
| Contagem | 4 bytes (int) | Quantidade de bytes únicos na tabela |
| Tabela | N × 5 bytes | Pares `[Byte (1 byte) + Frequência (4 bytes)]` |
| Bits Lixo | 1 byte | Bits não usados no último byte (0 a 7) |
| Payload | Variável | Dados codificados em bits |

---

## 💻 Como Compilar

É necessário ter o **GCC** instalado. No terminal:

```bash
gcc main.c MinHeap.c -o huffman
```

Ou, para compilar diretamente na pasta `output/`:

```bash
gcc main.c MinHeap.c -o output/huffman
```

---

## 🚀 Como Executar

### Compactar um arquivo

```bash
./huffman -c <arquivo_entrada> <arquivo_saida.huff>
```

**Exemplo:**

```bash
./huffman -c documento.txt documento.huff
```

### Descompactar um arquivo

```bash
./huffman -d <arquivo.huff> <arquivo_restaurado>
```

**Exemplo:**

```bash
./huffman -d documento.huff documento_restaurado.txt
```

---

## ⚙️ Como Funciona

### Compressão (flag `-c`)

1. **Contagem de frequências** — Lê o arquivo byte a byte e conta quantas vezes cada byte (0–255) aparece
2. **Fila de prioridade** — Cria uma lista encadeada ordenada por frequência crescente (Min-Heap)
3. **Árvore de Huffman** — Remove os dois nós de menor frequência, cria um pai com a soma, e reinsere. Repete até sobrar 1 nó (a raiz)
4. **Geração de códigos** — Percorre a árvore recursivamente: esquerda = `0`, direita = `1`. Ao chegar em uma folha, salva o código na Tabela Hash
5. **Gravação do `.huff`** — Escreve o cabeçalho (contagem + tabela + bits lixo) e depois codifica o arquivo original bit a bit

### Descompressão (flag `-d`)

1. **Leitura do cabeçalho** — Lê o número de símbolos e a tabela de frequências
2. **Reconstrução da árvore** — Usa as mesmas frequências para reconstruir a árvore idêntica
3. **Decodificação** — Lê o payload bit a bit, percorrendo a árvore da raiz até uma folha para recuperar cada byte original

---

## 📊 Resultados de Compactação

| Tipo de Arquivo | Tamanho Original | Tamanho Compactado | Taxa de Compressão |
|---|---|---|---|
| Texto pequeno com alta repetição (16 chars) | 17 bytes | 29 bytes | -70,59% (esperado*) |
| Texto grande (~276 KB) | 276.000 bytes | 153.585 bytes | **44,35%** |
| Arquivo binário (.exe, ~50 KB) | 50.269 bytes | 36.895 bytes | **26,60%** |

> \* Arquivos muito pequenos ficam maiores após a compressão porque o cabeçalho do `.huff` ocupa mais espaço do que a economia obtida na codificação. Isso é comportamento normal e esperado do algoritmo.

**Em todos os 3 testes, o arquivo restaurado foi verificado como idêntico ao original (sem perda de dados).**

---

## 🧪 Testes Realizados

- ✅ Arquivo de texto pequeno com alta repetição (`AAAAAAABBBBBCCCC`)
- ✅ Arquivo de texto grande (~276 KB, trecho de Dom Casmurro repetido)
- ✅ Arquivo binário (executável `.exe`)
- ✅ Verificação de integridade com `fc /b` (comparação byte a byte)

---

## 📌 Status do Projeto

✅ **Concluído** — Compressor e Descompressor funcionando corretamente.

---

## 📖 Disciplina

**Estrutura de Dados II**
Universidade Federal do Tocantins (UFT)

---

## 📄 Licença

Projeto acadêmico desenvolvido exclusivamente para fins educacionais.