#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Função auxiliar para imprimir o código Huffman
void printHuffmanCodes(struct HuffmanNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printHuffmanCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printHuffmanCodes(root->right, arr, top + 1);
    }
    if (isLeaf(root)) {
        printf("%c: ", root->character);
        for (int i = 0; i < top; ++i) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}

// Função principal
int main() {
    char characters[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned frequencies[] = { 5, 9, 12, 13, 16, 45 };
    int size = sizeof(characters) / sizeof(characters[0]);

    // Constrói a árvore de Huffman
    struct HuffmanNode* root = buildHuffmanTree(characters, frequencies, size);

    // Imprime os códigos Huffman
    int arr[100];
    int top = 0;
    printf("Códigos Huffman:\n");
    printHuffmanCodes(root, arr, top);

    return 0;
}
