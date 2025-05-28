
#include <stdio.h>
#include <stdlib.h>

/*  Single Linked List with 3 nodes :
 *
 * |10|NULL|
 *
 * addr1
 *
 * |10|addr2| |20|NULL|
 *
 * addr1       addr2
 *
 * |10|addr2| |20|addr3|   |30|Null|
 *
 *   addr1      addr2        addr3
 */

/*
 * self referential structure :
 *
 * -- self referential structures are those structures in which one or more pointers points
 *    to the structure of same type;
 *
 */
struct node
{
	int data;
	struct node *next;
};

struct node *head=NULL;

void create_single_list()
{
	/* creating a single list with 3 nodes */
	//struct node *ptr1;
	struct node *ptr2;
	struct node *ptr3;

	head = (struct node *)malloc(sizeof(struct node));

	head->data = 10;
	head->next = NULL;


	ptr2 = (struct node *)malloc(sizeof(struct node));

	ptr2->data = 20;
	ptr2->next = NULL;
	head->next = ptr2;

	ptr3 = (struct node *)malloc(sizeof(struct node));

	ptr3->data = 30;
	ptr3->next = NULL;
	//ptr3->next = head; to create a circular loop in list
	ptr2->next = ptr3;

	printf("single linked list created with 3 nodes : \n");

}

void print_single_linked_list()
{
	struct node *ptr;

	ptr = head; // head node is taken as global

	while(ptr!=NULL)
	//while((ptr!=NULL) && (ptr->next != head))
	{
       printf("%d \t",ptr->data);
       ptr = ptr->next;
	}

	printf("\n");
}

/* insert_node_at_beginning :
 *    10        20           30
 *
 * |10|addr2| |20|addr3|   |30|Null|
 *
 *   addr1      addr2        addr3
 *
 *
 *   5           10              20           30
 *
 *  |5|addr1|  |10|addr2|    |20|addr3|   |30|Null|
 *
 *  addr0          addr1         addr2        addr3
 *
 */

 /*
  * 1. to insert node, first create node and assign data to it,
  * 2. then map new address to existing link
  */

void *insert_node_at_beginning(struct node *head,int data)
{

	struct node *new_node = (struct node *)malloc(sizeof(struct node *));

	new_node->data = data;
	new_node->next = NULL;

	new_node->next = head;

	head = new_node;

	return head;

}

/*
 *  insert_node_at_ending :
 *    10        20           30
 *
 * |10|addr2| |20|addr3|   |30|Null|
 *
 *   addr1      addr2        addr3
 *
 *
 *    10              20           30         40
 *
 *  |10|addr2|    |20|addr3|   |30|addr4|    |40|Null|
 *
 *     addr1         addr2        addr3      addr4
 *
 */

/*
  * 1. to insert node, first create node and assign data to it,
  * 2. then map new address to existing link
  */


void insert_node_at_ending(struct node *head,int data)
{

	struct node *new_node; // for new_node

	struct node *ptr; // to traverse to end of list


	new_node = (struct node *)malloc(sizeof(struct node *));

	new_node->data = data;
	new_node->next = NULL;

	int node_count=0;

	if(head == NULL)
	{
		head = new_node;
		printf("...");
		return;
	}
	else
	{
		ptr = head;
	}

	//Note : here check condition ptr->next!=NULL wisely;
	while(ptr->next!=NULL)
	{
		node_count++;
		printf("node_count : %d\n",node_count);
		ptr = ptr->next;
	}

	ptr->next= new_node;

}

/* insert_node_at_particular_location :
 *
 *    10        20           30
 *
 * |10|addr2| |20|addr3|   |30|Null|
 *
 *   addr1      addr2        addr3
 *
 *
 *     10        20           25          30
 *
 * |10|addr2| |20|addr4|   |30|addr3|    |30|Null|
 *
 *   addr1      addr2        addr4       aadr3
 *
 */

/*
  * 1. to insert node, first create node and assign data to it,
  * 2. then map new address to existing link
  */

void insert_node_at_particular_location(struct node *head,int data,int position)
{
	int node_count =0;

	struct node *ptr = head ; //for traversing..

	struct node *new_node = (struct node *)malloc(sizeof(struct node));
	new_node->data = data;
	new_node->next = NULL;

	while(ptr!=NULL)
	{
		node_count++;
		printf("node_count: %d \n",node_count);

		if(node_count == position-1)
		{

			new_node->next = ptr->next;
			ptr->next = new_node;

			goto end_while;
		}
		else
		{
			ptr = ptr->next;

		}
	}

	end_while :
	printf("end of while \n");
}

void count_number_of_nodes_in_single_list()
{
	int nodes_count =0;
	struct node *ptr;

	ptr = head;

	while(ptr!=NULL)
	{
		nodes_count++;
		ptr = ptr->next;
	}

	printf("Number of node : %d\n",nodes_count);

}

/* delete first node :
 *
 *    10        20           30
 *
 * |10|addr2| |20|addr3|   |30|Null|
 *
 *   addr1=head      addr2        addr3
 *
 *
 *         20            30
 *
 * |20|addr3|       |30|Null|
 *
 *    addr2= head         addr3
 *
 */
void delete_first_node()
{
	struct node *ptr;

	ptr = head;

	head = ptr->next;

	free(ptr);


}

/* delete last node :
 *
 *    10        20           30
 *
 * |10|addr2| |20|addr3|   |30|Null|
 *
 *   addr1=head      addr2        addr3
 *
 *
 * *   10        20
 *
 * |10|addr2| |20|Null|
 *
 *   addr1     addr2

 *
 *//*
void delete_last_node()
{
	int node_count=0;
	struct node *ptr;

	struct node *temp;

	ptr = head;


   while(ptr->next->next !=NULL)
   {
	   node_count++;
	   printf(" node_count :%d \n",node_count);
	   ptr = ptr->next;

   }

   temp = ptr->next;
   ptr->next = NULL;

  printf("%d\n",temp->data);

  free(temp); // deleting 3rd node

}*/

void delete_last_node()
{
	int node_count=0;
	struct node *ptr = head; // for traversing
	struct node *prev =NULL;

   while(ptr->next != NULL)
   {
	   node_count++;
	   printf(" node_count :%d \n",node_count);
	   prev = ptr;
	   ptr = ptr->next;

   }
   prev->next = NULL;
}

/* delete particular node :
 *
 *    10        20           30
 *
 * |10|addr2| |20|addr3|   |30|Null|
 *
 *   addr1=head      addr2        addr3
 *
 *
 *
 *    10              30
 *
 * |10|addr3|      |30|Null|
 *
 *   addr1=head     addr3
 *
 *
 *
 *
 */


void delete_particular_node(int position)
{
	int node_count =0;
	struct node *ptr;

	ptr = head;

	while(ptr!=NULL)
	{
		node_count++;
		if(node_count == position-1)
		{
			ptr->next = ptr->next->next;
            break;
		}
		else{
			ptr = ptr->next;
		}


	}

   printf("delete particular node done \n");

}




/* Reverse a Single Linked List :
 *
 *    10        20           30
 *
 * |10|addr2| |20|addr3|   |30|Null|
 *
 *   addr1=head      addr2        addr3
 *
 *
 *     10        20           30
 *
 * |10|NULL| |20|addr1|   |30|addr2|
 *
 *   addr1      addr2        addr3=head
 *
 *
 */

/*
	1.Initialize three pointers prev as NULL, curr as head and next as NULL.
	2.Iterate through the linked list. In loop, do following.
		// Before changing next of current,
		// store next node
		next = curr->next
		// Now change next of current
		// This is where actual reversing happens
		curr->next = prev
		// Move prev and curr one step forward
		prev = curr
		curr = next
 *
 */

void reverse_single_linked_list()
{

	struct node *current=head;
	struct node *next=NULL;
	struct node *previous=NULL;

	while(current!=NULL)
	{
		next = current->next;

		current->next = previous;

		previous = current;

		current = next;
	}

	head = previous;

}

/* slow ptr and fast ptr to detect loop in linked list*/
int find_loop(struct node *head)
{
	struct node *slow_ptr = head;

	struct node *fast_ptr = head;


	while(slow_ptr && fast_ptr && fast_ptr->next)
	{
		slow_ptr = slow_ptr->next;
		fast_ptr = fast_ptr->next->next;
		if(slow_ptr == fast_ptr)
		{
			printf("there is a loop in list \n");
			return 1;
		}
	}

	printf("no_loop \n");

}

void delete_node_having_data(struct node *head, int data)
{
	struct node *ptr = head;
	struct node *prev = head;

	while(ptr!=NULL)
	{
		if(ptr->data == data)
		{
			prev->next = ptr->next;
			break;
			//ptr = ptr->next;
		}
		else{
			prev = ptr;
			ptr = ptr->next;
		}

	}
}

int main()
{
	int choice;
	create_single_list();

	print_single_linked_list();

	printf("\n");

	printf("Choose Operation to perform on Single Linked List : \n");

	printf("1. Insert node at beginning :\n");
	printf("2. Insert Node at Ending :\n");
	printf("3. Insert Node at particular location :\n");

	printf("4. Count Number of Nodes :\n");

	printf("5. Delete first node :\n");
	printf("6. Delete last node :\n");
	printf("7. Delete a particular node : \n");

	printf("8. Reverse a Single LInked List : \n");
	printf("9. Exit \n");
	printf("10. find loop in code \n");
	printf("11. delete node having data");

	scanf("%d",&choice);
	switch(choice)
	{
		case 1 :
			head = insert_node_at_beginning(head,5);

			printf("\n");
			printf("Linked list after inserting node at the beginning \n");
			print_single_linked_list();

			return 0;

		break;

		case 2 :
			insert_node_at_ending(head,40);

			printf("\n");
			printf("Linked list after inserting node at the ending \n");
			print_single_linked_list();

			return 0;


		case 3 :
			insert_node_at_particular_location(head,25,3);

			printf("\n");
			printf("Linked list after inserting node at particular Location \n");
			print_single_linked_list();

			return 0;

		case 4 :

			count_number_of_nodes_in_single_list();

			return 0;

		case 5 :
			delete_first_node();

			printf("\n");
			printf("Linked list after deleting first node :\n");
			print_single_linked_list();

			return 0;

		case 6 :
			delete_last_node();

			printf("\n");
			printf("Linked list after deleting Last node :\n");
			print_single_linked_list();

			return 0;

		case 7 :
			delete_particular_node(3);

			printf("\n");
			printf("Linked list after deleting Particular node :\n");
			print_single_linked_list();

			return 0;

		case 8 :
			reverse_single_linked_list();

			printf("\n");
			printf("Linked list after Reversing :\n");
			print_single_linked_list();

			return 0;

		case 9 :

			break;

		case 10 :

			find_loop(head);
			return 0;

		case 11 :
			delete_node_having_data(head,20);
			print_single_linked_list();
			return 0;
	}


	printf("exited \n");
	return 0;
}
