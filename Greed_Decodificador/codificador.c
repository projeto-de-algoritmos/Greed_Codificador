#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Estrutura para representar um nó da árvore de Huffman
struct HuffmanNode {
    char character;
    unsigned frequency;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
};

// Função auxiliar para criar um novo nó da árvore de Huffman
struct HuffmanNode* createNode(char character, unsigned frequency) {
    struct HuffmanNode* node = (struct HuffmanNode*)malloc(sizeof(struct HuffmanNode));
    node->character = character;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Função auxiliar para verificar se um nó é uma folha da árvore de Huffman
int isLeaf(struct HuffmanNode* node) {
    return !(node->left) && !(node->right);
}

// Função de comparação para usar com a fila de prioridade (heap)
int compareNodes(const void* a, const void* b) {
    struct HuffmanNode* nodeA = *(struct HuffmanNode**)a;
    struct HuffmanNode* nodeB = *(struct HuffmanNode**)b;
    return (nodeA->frequency) - (nodeB->frequency);
}

// Função para construir a árvore de Huffman a partir dos caracteres e frequências
struct HuffmanNode* buildHuffmanTree(char characters[], unsigned frequencies[], int size) {
    struct HuffmanNode *left, *right, *top;

    // Cria um heap para os nós da árvore
    struct HuffmanNode** heap = (struct HuffmanNode**)malloc(size * sizeof(struct HuffmanNode*));
    for (int i = 0; i < size; ++i) {
        heap[i] = createNode(characters[i], frequencies[i]);
    }
    int heapSize = size;

    // Constroi a árvore de Huffman
    while (heapSize > 1) {
        // Extrai os dois nós de menor frequência do heap
        left = heap[0];
        right = heap[1];
        // Cria um novo nó pai com a soma das frequências dos dois nós
        top = createNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;

        // Remove os dois nós de menor frequência do heap
        heap[0] = top;
        heap[1] = heap[heapSize - 1];
        --heapSize;

        // Reorganiza o heap
        int parent = 0;
        int child = 1;
        while (child < heapSize) {
            if (child + 1 < heapSize && compareNodes(&heap[child], &heap[child + 1]) > 0)
                child++;
            if (compareNodes(&heap[parent], &heap[child]) <= 0)
                break;
            struct HuffmanNode* temp = heap[parent];
            heap[parent] = heap[child];
            heap[child] = temp;
            parent = child;
            child = parent * 2 + 1;
        }
    }

    // O último nó restante no heap é a raiz da árvore de Huffman
    struct HuffmanNode* root = heap[0];
    free(heap);

    return root;
}

// Função auxiliar para imprimir a sequência de binários entre colchetes
void printBinaryString(char* binaryString) {
    printf("[");
    for (int i = 0; binaryString[i] != '\0'; i++) {
        printf("%c", binaryString[i]);
    }
    printf("]");
}

// Função auxiliar para imprimir os códigos Huffman e suas representações binárias
void printHuffmanCodes(struct HuffmanNode* root, int arr[], int top, char* binaryStrings[]) {
    if (root->left) {
        arr[top] = 0;
        printHuffmanCodes(root->left, arr, top + 1, binaryStrings);
    }
    if (root->right) {
        arr[top] = 1;
        printHuffmanCodes(root->right, arr, top + 1, binaryStrings);
    }
    if (isLeaf(root) && root->character != '\0') {
        binaryStrings[(unsigned char)root->character] = (char*)malloc((top + 1) * sizeof(char));
        for (int i = 0; i < top; ++i) {
            binaryStrings[(unsigned char)root->character][i] = arr[i] + '0';
        }
        binaryStrings[(unsigned char)root->character][top] = '\0';

        if (root->character == ' ') {
            printf(" : ");
        } else {
            printf("%c: ", root->character);
        }
        printBinaryString(binaryStrings[(unsigned char)root->character]);
        printf("\n");
    }
}

// Função principal
int main() {
    char characters[256];
    char string[10001];  // Array para armazenar a string lida (com espaço para 10000 caracteres)
    printf("Digite uma string: ");
    scanf("%[^\n]%*c", string);  // Lê a string até o caractere de nova linha

    // Armazena os caracteres únicos da string
    bool caracteres[256] = { false };
    int numCaracteres = 0;
    for (int i = 0; string[i] != '\0'; i++) {
        if (!caracteres[(unsigned char)string[i]]) {
            characters[numCaracteres++] = string[i];
            caracteres[(unsigned char)string[i]] = true;
        }
    }

    // Constrói a árvore de Huffman
    int frequencias[numCaracteres];
    for (int i = 0; i < numCaracteres; i++) {
        int contador = 0;
        for (int j = 0; string[j] != '\0'; j++) {
            if (string[j] == characters[i]) {
                contador++;
            }
        }
        frequencias[i] = contador;
    }
    struct HuffmanNode* root = buildHuffmanTree(characters, frequencias, numCaracteres);

    // Armazena as sequências de código Huffman de cada caractere
    char* binaryStrings[256] = { NULL };
    int arr[100];
    int top = 0;
    printHuffmanCodes(root, arr, top, binaryStrings);

    // Substitui cada caractere da string pela sua sequência de código Huffman entre colchetes
    printf("Representações binárias:\n");
    for (int i = 0; string[i] != '\0'; i++) {
        if (binaryStrings[(unsigned char)string[i]] != NULL) {
            printBinaryString(binaryStrings[(unsigned char)string[i]]);
        }
    }
    printf("\n");

    // Libera a memória alocada para as sequências de código Huffman
    for (int i = 0; i < numCaracteres; i++) {
        if (binaryStrings[(unsigned char)characters[i]] != NULL) {
            free(binaryStrings[(unsigned char)characters[i]]);
        }
    }

    return 0;
}