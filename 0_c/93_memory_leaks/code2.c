/*1.Weather program has memory leak or not (using a simple using counter)
2.If program has memory leak then where (need linked list to trace all calls)*/

/*
 * This approach is very simple. We take a global counter and initialize it to zero at the beginning.
 * Now we modify the definition of malloc and free with our private functions.
 * During allocation we allocate through malloc and after a success we increment this counter.
 * Same way during free we free the chunk and decrement the counter.

A memory leak free program should call as much malloc as free at the end.
So this counter will be zero at exit.
If this counter is above zero then programs has memory leak and the value of the counter will tell the number of chunks that are not freed.
my_malloc() is our custom allocator and my_free() is our custom deallocator.
 We have replaced the definition of malloc and free with these functions.
So our application program will call these routines instead of original malloc and free of C library.
 */

#include<stdio.h>
#include<stdlib.h>

int mem_counter = 0;

void my_free(void * p)
{
  free(p);
  printf ("Free memory %p \n", p);
  printf ("Current leak counter %d\n", --mem_counter);
}

void * my_malloc(int size)
{
  void * ret;
  ret = malloc(size);
  if(ret) {
    printf ("Allocated memory @%p of size %d \n", ret, size);
    printf ("Current leak counter %d\n", ++mem_counter);
  }
  return ret;
}

#undef malloc
#define malloc(size) my_malloc(size)
#undef free
#define free(p) my_free(p)


/*user code starts here - We are detecting leak here*/

void some_task1(void)
{
  void * p;
  printf("Task 1\r\n");
  p = malloc(512);
  if(p)
    free(p);
}

void some_task2(void)
{
  void * p;
  printf("Task 2\r\n");
  p = malloc(256);

  /*free(p);*/ /* << memory leak added here*/
}

void some_task3(void)
{
  void * p;
  printf("Task 3\r\n");
  p = malloc(20);

  if(p)
    free(p);
}

int main(int argc, char *argv[])
{
  printf("Memory leak detect using counter :\r\n\n");
  some_task1();
  printf("\n");
  some_task2();
  printf("\n");
  some_task3();
  printf("\n");
  if (mem_counter > 0)
    printf("memory leak detected, %d of chunk(s) not freeded\n", mem_counter);
  return 0;
}
