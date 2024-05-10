#include <stdio.h>
#include "declarations.h"

int main() {
    Client* head = NULL;

    insert(&head, 3);
    insert(&head, 5);
    insert(&head, 7);
    insert(&head, 9);

    printf("Linked List: ");
    print_list(head);

    delete(&head, 5);

    printf("Linked List after deletion: ");
    print_list(head);

    free_list(head);

    return 0;
}
