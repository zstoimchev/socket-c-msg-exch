#ifndef DECLARATIONS_H
#define DECLARATIONS_H

typedef struct Client {
    int data;
    struct Client* next;
} Client;

Client* create(int data);

void insert(Client** head_ref, int data);

void delete(Client** head_ref, int key);

void broadcast(Client** head, int source, char* result);

void print_list(Client* head);

void free_list(Client* head);

#endif
