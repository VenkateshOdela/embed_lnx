

/* Write a C program having all types of symbols
	* data
		* global initialized
		* global uninitialized
		* file - static initialized
		* file - static uninitialized
		* function - static initialized
		* function - static uninitialized
		* local initialized
		* local uninitialized
		* All the above with const qualifier
	* functions
		* global function
		* static function
*/

#include <stdio.h>


void func();

void gbl_func();
static void stat_func();

int gbl_initvar = 10; // .data
static int stat_initvar = 20; // .data

int gbl_uninitvar;  // ?? (.bss of a.out but not in .bss of .o) 
static int stat_uninitvar; // .bss
const static int const_stat_uninitvar; // .bss

const int const_gbl_initvar = 10; // .rodata
const static int const_stat_initvar = 20; // .rodata

const int const_gbl_uninitvar; // ?? (.bss of a.out but not in .bss of .o) 


void func()
{
 
 static int func_stat_initvar = 20; // .data(initiliazed static varaible)
 static int func_stat_uninitvar; // .bss(uninitialized static varaible;)
 
}

void gbl_func()
{
 printf("global function called \n");
}

void stat_func()
{
 printf("static function called \n");
}


int main() // .text
{
	int local_initvar =30; // stack :this local variables appear in runtime
    int local_uninitvar;  //  stack :this local variables appear in runtime
           
    func(); // .text

	gbl_func(); //.text

	stat_func(); // .text
 
	return 0;

}


/*
     stack
   
      ..

     heap
  
     .bss (unintilaised global and static varaibles)
 
     .data (data segment) (initliased global and static varaibles)

     .text (cs : code segment)
*/

/*
   $ gcc -c mem_layout.c

    mem_layout.o is generated;
 
    using objdump to see various sections in memory layout:
   
    objdump -D mem_layout.o
    
    sections .text, .data ,.bss , .rodata, .comment;
  
   
   3.  .rodata --> constants

   2.  .bss  --> Uninitialized global and static varaibles.(bss)

   1.  .data --> initialized global and static variables (data segment)

   0. .text --> instructions (code segment);

*/



/* *?

/*
Stack grows in the direction opposite to heap. 
Stack  :Last in First Out; 
*/
