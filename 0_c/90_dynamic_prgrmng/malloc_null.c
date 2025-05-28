#include <stdio.h>
#include <stdlib.h>

#define FRAME_SIZE 4*1024
 
int main()
{
 
    // This pointer will hold the
    // base address of the block created
    int *ptr = NULL;
    int n=1024, i;

    ptr = (int*)malloc(n * sizeof(int));

    int page_count =0;

    while (ptr != NULL)
    {
	
        printf("%p \n",ptr);
        page_count++;
        // Dynamically allocate memory using malloc()
	    //ptr = (int*)malloc(n * sizeof(int));
        printf("%d \n",page_count);
        ptr = (int*)malloc(FRAME_SIZE);
        
     }
 
    // Check if the memory has been successfully
    // allocated by malloc or not
    if (ptr == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
   
 
    return 0;
}
