
#include <stdio.h>

#if 1

#define my_sizeof(a)  (char*)(&a+1)- (char*)(&a)


int main()
{
 //int a[10];
 int a;
 printf("%ld \n",my_sizeof(a));
 return 0;
}

// output : 40


#if 0
int main()
{
 int a[10];
 int *ptr;
 ptr =&a;
 int b = ptr;
 int c = ptr+10;
 int d = c-b;
 
 //printf("%d\n",ptr);
 //printf("%d\n",ptr++);
 //printf("%d\n",ptr);
 //int d = ptr+1 - ptr;
 printf("%d \n",d);
 return 0;
}

// output : 40

#endif

#endif


#if 0
int main()
{
 int a;
 printf("%d\n",sizeof(a));
 return 0;
}
#endif

#if 0
/* finding size of array = finding number of elements in array */
#include <stdio.h>
 
int main(void)
{
    //int arr[] = { 1, 2, 3, 4, 5 };
    int arr[10];
    size_t n = sizeof(arr)/sizeof(arr[0]);
 
    printf("The number of elements in array is %d", n);
 
    return 0;
}
#endif
