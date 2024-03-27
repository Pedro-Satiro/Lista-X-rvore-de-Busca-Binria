#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct
{
    double time;
    int steps;
} Result;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }

    return root;
}

void printTree(Node* root) {
    if (root != NULL) {
        printTree(root->left);
        printf("%d ", root->data);
        printTree(root->right);
    }
}

int search(Node* root, int data, int steps) {
    if (root == NULL) {
        return -1; 
    }

    steps++; 
    if (data == root->data) {
        return steps; 
    }
    if (data < root->data) {
        return search(root->left, data, steps);
    } else {
        return search(root->right, data, steps);
    }
}

void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

Result procura_arvore(long long int *lista, int number_of_elements, int searchValue) {
    Node* root = NULL;
    // Arvore definida
    for (int i = 0; i < number_of_elements; i++)
        root = insert(root, lista[i]);


    // printf("arvore de busca binaria em pre-ordem:\n");
    // printTree(root);
    // printf("\n");

    LARGE_INTEGER frequency;
    LARGE_INTEGER start_time;
    LARGE_INTEGER end_time;
    double seconds;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);

    int steps = search(root, searchValue, 0);

    QueryPerformanceCounter(&end_time);
    seconds = (double)(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;
    // if (steps == -1) {
    //     printf("Valor nao encontrado na arvore.\n");
    // } else {
    //     printf("Valor encontrado na arvore em %d passos.\n", steps - 1 );
    // }
    freeTree(root);

    Result result = {seconds, steps};

    return result;
}

int main()
{
    int number_of_elements = 99999999;
    long long int* lista = malloc(number_of_elements * sizeof(long long int));
    srand(time(NULL));
    for (int i = 0; i < number_of_elements; i++)
    {
        lista[i] = rand();
    }
    Result result = procura_arvore(lista, number_of_elements, 410);
    printf("Tempo de execucao: %.20f s\n", result.time);
    printf("Numero de passos: %d\n", result.steps);
    return 0;
}