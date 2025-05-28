#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
	int data;
	char name[30];
	struct node *prev;
	struct node *next;
};


struct node *head = NULL;
struct node *tail = NULL;

struct node *temp = NULL;


//NULL - 10 - 20 - 30 - NULL

void create_single_list()
{
	struct node *ptr = (struct node *)malloc(sizeof(struct node));

	ptr->data = 10;
	strcpy(ptr->name,"venkatesh");
	ptr->prev=NULL;
    ptr->next=NULL;


    struct node *ptr2 = (struct node *)malloc(sizeof(struct node));

    ptr2->data = 20;
    strcpy(ptr2->name,"suresh");
    ptr2->prev=NULL;
    ptr2->next=NULL;

    ptr->next=ptr2;
    ptr2->prev=ptr;

    struct node *ptr3 = (struct node *)malloc(sizeof(struct node));

    ptr3->data = 30;
    strcpy(ptr3->name,"NARESH");
    ptr3->prev= NULL;
    ptr3->next = NULL;

    ptr2->next=ptr3;
    ptr3->prev= ptr2;

    head=ptr; // assuming first node as head
    tail=ptr3; //assuming last node as tail

    //ptr3->next =head; //making double linked list circular

}

void iscircular(struct node *head)
{
	struct node *temp = NULL;

	temp = head->next;

	/*
	 *  In double linked list
	 *   - Traverse through end of list, if it is NULL, Then it is not circular.
	 *   - Traverse through end of list, if it is again reached to head, Then it is CIRCULAR;
	 *
	 */

	while((temp!=NULL) && (temp!=head))
	{
		temp = temp->next;
	}

	if(temp==NULL)
	{
		printf("linked list is not circular\n");
	}

	if(temp == head)
	{
		printf("linked list is circular\n");
	}

}

int main()
{
	create_single_list();

	/*
	for(temp=tail;temp!=head;temp=temp->prev)
	{

		printf("data =%d \n",temp->data);
		printf("name =%s \n",temp->name);
	}
	printf("loop exit \n");
	*/

    iscircular(head);


}
