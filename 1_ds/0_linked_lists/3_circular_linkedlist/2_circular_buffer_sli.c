#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 5

/* single linked list node */
struct node {
    int data;
    struct node *next;
};

struct node *head = NULL;
int count = 0;

void insert_node(int data)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
        head->next = head;
    } else {
        new_node->next = head->next;
        head->next = new_node;
        head = new_node;
    }

    count++;

    if (count > MAX_NODES) {
        struct node *temp = head->next;
        head->next = temp->next;
        free(temp);
        count--;
    }
}

int main() {
    for (int i = 1; i <=6 ; i++) {
        insert_node(i);
    }

    struct node *temp = head->next;
    while (temp != head) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("%d\n", temp->data);

    return 0;
}
