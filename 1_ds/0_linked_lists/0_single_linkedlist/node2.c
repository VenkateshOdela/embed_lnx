/*
 * Creating Single Linked list node and traverse through node
 *
 * |data|link|
 *
 * |10|addr2|  |20|addr3| |30|addr4| |40|addr5| |50|null|
 *
 * addr1
 *
 */

#include <stdio.h>
#include <stdlib.h> // for malloc


struct node
{
	int data; //data in node
	struct node *next; // pointer to structure
};

struct node *head = NULL;

void beginsert ();


void last_insert()
{
	int data;
	struct node *ptr;
	ptr = (struct node *)malloc(sizeof(struct node));

	printf("enter data : \n");
	scanf("%d",&data);

	ptr->data = data;
	head = ptr;
	ptr->next = NULL;

}

/*
   |50|null| |40|addr5| |30|addr4| |20|addr3| |10|addr2|

                                                addr1
-----------------
|10|null|

addr1

|20|addr1| |10|NUll|

addr2       addr1

|30|addr2| |20|addr1| |10|NUll|

 addr3           addr2       addr1


*/


void beginsert()
{
	int data;
	struct node *ptr;

	ptr = (struct node *) malloc(sizeof(struct node));

	printf("enter data : \n");
    scanf("%d",&data);

	ptr->data = data;
	ptr->next = head;

	head = ptr;

	printf("\nNode inserted");

}

void display()
{

	struct node *ptr;

	ptr = head;

	while(ptr!=NULL)
	{
		printf("%d \t",ptr->data);
		ptr = ptr->next;
	}
	printf("\n");

}

/*
|10|addr2|  |20|addr3| |30|addr4| |40|addr5| |50|null|

addr1
-----------------
|10|null|

addr1

|10|addr2| |20|NUll|

addr1       addr2

|10|addr2| |20|addr3|   |30|NULL|

addr1       addr2        addr3
*/

void lastinsert()
{
	int data;
	struct node *ptr;

	ptr = (struct node *)malloc(sizeof(struct node));

	printf("enter data : \n")
	scanf("%d"&data);

	ptr->data = data;
	ptr->next = head;


}


int main()
{

	int choice=0;
	while(choice !=9)
	{
		printf("enter your choice \n");
		printf("1.begin insert \n");
		printf("2.display nodes in list\n");

		scanf("%d",&choice);

		switch(choice)
		{
		  case 1 :
			  beginsert();
			  break;
		  case 2 :
			  display();
			  break;
		  case 3 :
			  lastinsert();
			  break;

		  default :
			  printf("enter valid choice \n");
			  break;
		}
	}



}
