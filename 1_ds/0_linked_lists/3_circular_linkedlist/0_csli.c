#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
	int data;
	char name[30];
	struct node *next;
};


struct node *head = NULL;


void create_single_list()
{
	struct node *ptr = (struct node *)malloc(sizeof(struct node));

	ptr->data = 10;
	strcpy(ptr->name,"venkatesh");
    ptr->next=NULL;

    struct node *ptr2 = (struct node *)malloc(sizeof(struct node));

    ptr2->data = 20;
    strcpy(ptr2->name,"suresh");
    ptr2->next= NULL;
    ptr->next = ptr2;

    struct node *ptr3 = (struct node *)malloc(sizeof(struct node));

    ptr3->data = 30;
    strcpy(ptr3->name,"NARESH");
    ptr3->next= ptr; // assigning first node address to last node next address: making circular
    //ptr3->next= NULL; // NOT Circular
    ptr2->next = ptr3;

    head=ptr; // assuming first node as head
}

void iscircular(struct node *head)
{
	struct node *temp=NULL;

	temp=head->next;

	/*
	 *  In signgle list
	 *   - Traverse through end of list, if it is NULL, Then it is not circular.
	 *   - Traverse through end of list, if it is again reached to head, Then it is CIRCULAR;
	 *
	 */
	while((temp!=NULL)&&(temp!=head))
	{
		temp = temp->next;
	}

	if(temp==NULL)
	{
		printf("not circular\n");
	}

	if(temp==head)
	{
		printf("yes,it is circular\n ");
	}

}

int main()
{
	create_single_list();
   /*
	for(temp=head;temp->next!=head;temp=temp->next)
	{

		printf("data =%d \n",temp->data);
		printf("name =%s \n",temp->name);
	}*/
	//printf("loop exit \n");

	iscircular(head);

}

/*
 *  In signgle list
 *   - Traverse through end of list, if it is NULL, Then it is not circular.
 *   - Traverse through end of list, if it is again reached to head, Then it is CIRCULAR;
 *
 */

