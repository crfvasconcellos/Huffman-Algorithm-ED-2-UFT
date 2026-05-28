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
├── data/           # Arquivos de entrada para teste
│   ├── ABC.txt
│   ├── Adobe Express - file.png
│   └── casmurro.txt
├── output/         # Executável compilado e arquivos gerados nos testes
│   ├── huffman.exe
│   ├── ABC.huff
│   ├── ABC_restored.txt
│   ├── Adobe Express - file.huff
│   ├── Adobe Express - file_restored.png
│   ├── casmurro.huff
│   └── casmurro_restored.txt
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

| Arquivo | Tipo de Conteúdo | Tamanho Original | Tamanho Compactado | Taxa de Compressão |
|---|---|---|---|---|
| `ABC.txt` | Texto curto e repetitivo | 16 bytes | 24 bytes | -50,00% (esperado*) |
| `casmurro.txt` | Texto longo literário (Dom Casmurro) | 392.026 bytes | 226.690 bytes | **42,17%** |
| `Adobe Express - file.png` | Imagem binária pré-comprimida | 623.497 bytes | 624.782 bytes | -0,21% (esperado*) |

> \* **Nota sobre taxas negativas**: Arquivos extremamente pequenos (como `ABC.txt`) ou já previamente compactados (como imagens `.png`) podem apresentar um leve aumento no tamanho final. Isso ocorre porque o cabeçalho do formato `.huff` (que carrega as frequências dos símbolos necessárias para a descompressão) ocupa um espaço fixo que anula ou supera o ganho de bits na codificação. Este comportamento é previsto e normal para o algoritmo de Huffman estático.

**Em todos os 3 testes, a integridade dos arquivos originais e restaurados foi de 100% (sem perda de dados).**

---

## 🧪 Testes Realizados

- ✅ **Texto curto e repetitivo** (`data/ABC.txt`)
- ✅ **Texto longo literário** (`data/casmurro.txt` - obra completa Dom Casmurro)
- ✅ **Arquivo de imagem binária pré-comprimida** (`data/Adobe Express - file.png`)
- ✅ **Verificação de integridade byte a byte** usando `fc /b` para todos os arquivos, comprovando 100% de precisão na restauração.

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