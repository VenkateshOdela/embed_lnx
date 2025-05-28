/*
 * Creating Single Linked list node and traverse through node
 *
 * |data|link|
 *
 * |10|Null|
 *
 *
 */

#include <stdio.h>
#include <stdlib.h> // for malloc


struct node
{
	int data; //data in node
	struct node *next; // pointer to structure
};

// receive address of node and print the data of node;
void traverse_list(struct node *ptr)
{
	if(ptr == NULL)
	{
		printf("nothing to print\n");
	}else
	{
		while(ptr!=NULL)
		{
			printf("%d\n",ptr->data);
			ptr = ptr->next;
		}
	}
}


int main()
{
	struct node *head = NULL; // NUll: no memory assigned to node

	head = (struct node *)malloc(sizeof(struct node)); // memory is assigned to node using malloc

	printf("%p \n",head); //printing address of node
    head->data = 10;
    head->next = NULL;

    traverse_list(head); // passing address of node

}
