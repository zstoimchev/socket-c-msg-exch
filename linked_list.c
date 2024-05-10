#include <stdio.h>
#include <stdlib.h>
#include "declarations.h"
#include <string.h>
#include <sys/socket.h>

Client* create(int data) {
    Client* new = (Client*)malloc(sizeof(Client));
    if (new == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    new->data = data;
    new->next = NULL;
    return new;
}

void insert(Client** head_ref, int data) {
    Client* new = create(data);
    new->next = *head_ref;
    *head_ref = new;
}

void delete(Client** head_ref, int key) {
    Client* temp = *head_ref;
    Client* prev = NULL;

    if (temp != NULL && temp->data == key) {
        *head_ref = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    prev->next = temp->next;
    free(temp);
}

void print_list(Client* head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

void broadcast(Client** head, int source, char* result) {
  while(*head != NULL) {
    send((*head)->data, result, strlen(result), 0);
    *head = (*head)->next;
  }
}

void free_list(Client* head) {
    Client* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
