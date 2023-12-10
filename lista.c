#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Função para criar um novo nó
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erro ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Função para inserir um novo nó no início da lista
void insertAtBeginning(Node** head, int data) {
    Node* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
}

// Função para remover um nó com um valor específico
void removeNode(Node** head, int targetValue) {
    Node* current = *head;
    Node* prev = NULL;

    // Percorre a lista até encontrar o nó com o valor alvo
    while (current != NULL && current->data != targetValue) {
        prev = current;
        current = current->next;
    }

    // Se o nó com o valor alvo foi encontrado
    if (current != NULL) {
        // Se o nó é o primeiro da lista
        if (prev == NULL) {
            *head = current->next;
        } else {
            // Se o nó não é o primeiro da lista
            prev->next = current->next;
        }

        // Libera a memória do nó removido
        free(current);
    }
}

// Função para imprimir os elementos da lista
void printList(Node* head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

// Função para liberar a memória alocada para a lista encadeada
void freeList(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Node* head = NULL; // Lista vazia no início

    // Inserindo alguns elementos no início da lista
    insertAtBeginning(&head, 3);
    insertAtBeginning(&head, 7);
    insertAtBeginning(&head, 9);

    // Imprimindo a lista
    printf("Lista inicial: ");
    printList(head);

    // Adicionando mais elementos
    insertAtBeginning(&head, 5);
    insertAtBeginning(&head, 2);

    // Imprimindo a lista novamente
    printf("Lista após adição: ");
    printList(head);

    // Remover o nó com o valor 7 da lista
    removeNode(&head, 7);

    // Imprimir a lista após a remoção
    printf("Lista após remoção do valor 7: ");
    printList(head);

    // Liberando a memória alocada para a lista
    freeList(head);

    return 0;
}
