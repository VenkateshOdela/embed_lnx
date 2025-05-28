#include <stdio.h>
#include <stdlib.h>

//global_data_structure

#define MAX_ALLOCAIONS 10

struct each_alloc_info
{
	void* address;
	int size;
	int line;
};


struct gds
{
    struct each_alloc_info arr[MAX_ALLOCAIONS];
	int total_allocated_bytes;
	int total_freed_bytes;
};

// as it is global data structure, assuming it is initialized to zero;
struct gds gobj;

struct each_alloc_info *get_next_free_arr_address()
{
	for(int i=0;i<MAX_ALLOCAIONS;i++)
	{
         if(gobj.arr[i].address == 0)
         {
        	 return &gobj.arr[i];
         }
	}

	return NULL;
}

struct each_alloc_info *check_addr_in_arr(void *address)
{
	for(int i=0;i<MAX_ALLOCAIONS;i++)
	{
		 if(gobj.arr[i].address == address)
		 {
			 return &gobj.arr[i];
		 }
	}

	return NULL;

}

void *my_malloc(int size,int line)
{

	void *start_address = malloc(size);

    struct each_alloc_info *temp = get_next_free_arr_address();

    if(temp == NULL)
    {
    	printf("gds is full, maximum allocations reached \n");
    	return 0;
    }

    temp->address = start_address;
    temp->size = size;
    temp->line = line;

    gobj.total_allocated_bytes = gobj.total_allocated_bytes + size;

    return start_address;

}

void my_free(void *address,int line)
{
	if(address == NULL)
	{
		printf("trying to free the null pointer \n");
		return;
	}

	struct each_alloc_info *temp = check_addr_in_arr(address);

	if(temp == NULL)
	{
		printf("double free at line :%d \n",line);
	}else{
		free(address);

		gobj.total_freed_bytes = gobj.total_freed_bytes + temp->size;
		temp->address =0;
		temp->size =0;
		temp->line =0;
	}
}

void print_report()
{

	printf("total_allocated_bytes :%d \n",gobj.total_allocated_bytes);
	printf("total_freed_bytes :%d \n",gobj.total_freed_bytes);
	printf("\n");

	if(gobj.total_allocated_bytes != gobj.total_freed_bytes)
	{
		printf("MEMORY LEAK DETECTED\n");

		for(int i=0;i<MAX_ALLOCAIONS;i++)
		{
			 if(gobj.arr[i].address != 0)
			 {
				 printf("memory leak at line: %d \n",gobj.arr[i].line);
			 }
		}
	}else{
		printf("NO memory leak \n");
	}

}

int main()
{


	char *ptr = my_malloc(10*sizeof(char),__LINE__);

	my_free(NULL,__LINE__);
	/*
	my_free(ptr,__LINE__);


	char *ptr1 = my_malloc(10*sizeof(char),__LINE__);
	my_free(ptr1,__LINE__);
	char *ptr2 = my_malloc(10*sizeof(char),__LINE__);
	char *ptr3 = my_malloc(10*sizeof(char),__LINE__);
	my_free(ptr3,__LINE__);
	my_free(ptr3,__LINE__);
	char *ptr4 = my_malloc(10*sizeof(char),__LINE__);
	char *ptr5 = my_malloc(10*sizeof(char),__LINE__);*/

	//my_free(ptr,__LINE__);

	print_report();
}
