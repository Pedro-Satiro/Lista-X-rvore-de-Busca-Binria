#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#define _POSIX_C_SOURCE 200809L

struct Node
{
    int data;
    struct Node *next;
};

typedef struct
{
    double time;
    int steps;
} Result;

void printList(struct Node *head)
{
    while (head != NULL)
    {
        // printf("%d -> ", head->data);
        head = head->next;
    }
    // printf("NULL\n");
}

struct Node *insertAtEnd(struct Node *tail, int newData)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = newData;
    newNode->next = NULL;

    tail->next = newNode;
    return newNode;
}

int searchValueInList(struct Node *head, int value, int *steps)
{
    struct Node *current = head;
    int position = 0;
    while (current != NULL)
    {
        (*steps)++;
        if (current->data == value)
        {
            return position;
        }
        current = current->next;
        position++;
    }
    return -1;
}

Result procurar_lista(long long int *lista, int number_of_elements, int searchValue)
{
    // Lista definida
    int size = number_of_elements;
    struct Node *head = NULL;
    struct Node *tail = NULL;
    if (size > 0)
    {
        head = (struct Node *)malloc(sizeof(struct Node));
        head->data = lista[0];
        head->next = NULL;
        tail = head;
    }
    for (int i = 1; i < size; i++)
        tail = insertAtEnd(tail, lista[i]);

    int steps = 0;
    int position = -1;

    LARGE_INTEGER frequency;
    LARGE_INTEGER start_time;
    LARGE_INTEGER end_time;
    double elapsed_time;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);

    position = searchValueInList(head, searchValue, &steps);

    QueryPerformanceCounter(&end_time);
    elapsed_time = (double)(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;

    // if (position != -1)
    //     printf("O valor %d foi encontrado e levou %d passos.\n", searchValue, steps);
    // else
    //     printf("O valor %d nao foi encontrado na lista.\n", searchValue);

    struct Node *current = head;
    while (current != NULL)
    {
        struct Node *nextNode = current->next;
        free(current);
        current = nextNode;
    }
    Result result = {elapsed_time, steps};

    return result;
}

int main()
{
    int number_of_elements = 99999999;
    long long int *lista = malloc(number_of_elements * sizeof(long long int));
    srand(time(NULL));
    for (long long int i = 0; i < number_of_elements; i++)
        lista[i] = rand();

    Result result = procurar_lista(lista, number_of_elements, 410);

    printf("Tempo de execucao: %.20f s\n", result.time);
    printf("Numero de passos: %d\n", result.steps);
    return 0;
}