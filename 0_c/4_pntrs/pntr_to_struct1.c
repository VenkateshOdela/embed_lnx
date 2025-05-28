/*
 * 1. Pointer to Structure
 *
 * 2. Note : object instance of structure is passed to
 *           pointer pointing to same structure type;
 *
 */
#include <stdio.h>

struct abc
{
	int a;
	int b;
	int c;
};

int main()
{
	struct abc obj; // structure object is created
	//struct abc *pntr = &obj; // pntr to structure object obj

	struct abc *pntr = NULL;
	pntr = &obj;

	pntr->a = 10;
	pntr->b = 20;
	pntr->c = 30;

	printf("%d\n",pntr->a);
	printf("%d\n",pntr->b);
	printf("%d\n",pntr->c);

	return 0;

}

/*
 * output:
 * 10
 * 20
 * 30
 *
 */
