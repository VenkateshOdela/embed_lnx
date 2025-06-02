

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

const int const_gbl_uninitvar; // ?? .rodata


void func()
{
 
 static int func_stat_initvar = 20; // .data(initiliazed static varaible)
 static int func_stat_uninitvar; // .bss(uninitialized static varaible;)
 
}

void gbl_func()
{
   static int gbl_func_stat_var_int =10; // .data initlaized static local variable.
   static int gbl_func_stat_var_unint; // .bss unintialized static local varaible.
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
  
     .bss (unintilaised global and static varaibles)(.bss block started by symbol)
 
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


/**
Disassembly of section .text:

0000000000000000 <func>:
   0:   f3 0f 1e fa             endbr64 
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   90                      nop
   9:   5d                      pop    %rbp
   a:   c3                      retq   

000000000000000b <gbl_func>:
   b:   f3 0f 1e fa             endbr64 
   f:   55                      push   %rbp
  10:   48 89 e5                mov    %rsp,%rbp
  13:   48 8d 3d 00 00 00 00    lea    0x0(%rip),%rdi        # 1a <gbl_func+0xf>
  1a:   e8 00 00 00 00          callq  1f <gbl_func+0x14>
  1f:   90                      nop
  20:   5d                      pop    %rbp
  21:   c3                      retq   

0000000000000022 <stat_func>:
  22:   f3 0f 1e fa             endbr64 
  26:   55                      push   %rbp
  27:   48 89 e5                mov    %rsp,%rbp
  2a:   48 8d 3d 00 00 00 00    lea    0x0(%rip),%rdi        # 31 <stat_func+0xf>
  31:   e8 00 00 00 00          callq  36 <stat_func+0x14>
  36:   90                      nop
  37:   5d                      pop    %rbp
  38:   c3                      retq   

0000000000000039 <main>:
  39:   f3 0f 1e fa             endbr64 
  3d:   55                      push   %rbp
  3e:   48 89 e5                mov    %rsp,%rbp
  41:   48 83 ec 10             sub    $0x10,%rsp
  45:   c7 45 fc 1e 00 00 00    movl   $0x1e,-0x4(%rbp)
  4c:   b8 00 00 00 00          mov    $0x0,%eax
  51:   e8 00 00 00 00          callq  56 <main+0x1d>
  56:   b8 00 00 00 00          mov    $0x0,%eax
  5b:   e8 00 00 00 00          callq  60 <main+0x27>
  60:   b8 00 00 00 00          mov    $0x0,%eax
  65:   e8 b8 ff ff ff          callq  22 <stat_func>
  6a:   b8 00 00 00 00          mov    $0x0,%eax
  6f:   c9                      leaveq 
  70:   c3                      retq   
  */

/*
  Disassembly of section .data:

0000000000000000 <gbl_initvar>:
   0:   0a 00                   or     (%rax),%al
        ...

0000000000000004 <stat_initvar>:
   4:   14 00                   adc    $0x0,%al
        ...

0000000000000008 <func_stat_initvar.2326>:
   8:   14 00                   adc    $0x0,%al
        ... */

/*
Disassembly of section .bss:

0000000000000000 <stat_uninitvar>:
   0:   00 00                   add    %al,(%rax)
        ...

0000000000000004 <func_stat_uninitvar.2327>:
   4:   00 00                   add    %al,(%rax)
*/


/*
Disassembly of section .rodata:

0000000000000000 <const_stat_uninitvar>:
   0:   00 00                   add    %al,(%rax)
        ...

0000000000000004 <const_gbl_initvar>:
   4:   0a 00                   or     (%rax),%al
        ...

0000000000000008 <const_stat_initvar>:
   8:   14 00                   adc    $0x0,%al
   a:   00 00                   add    %al,(%rax)
   c:   67 6c                   insb   (%dx),%es:(%edi)
   e:   6f                      outsl  %ds:(%rsi),(%dx)
   f:   62 61 6c 20 66          (bad)
  14:   75 6e                   jne    84 <main+0x4b>
  16:   63 74 69 6f             movslq 0x6f(%rcx,%rbp,2),%esi
  1a:   6e                      outsb  %ds:(%rsi),(%dx)
  1b:   20 63 61                and    %ah,0x61(%rbx)
  1e:   6c                      insb   (%dx),%es:(%rdi)
  1f:   6c                      insb   (%dx),%es:(%rdi)
  20:   65 64 20 00             gs and %al,%fs:(%rax)
  24:   73 74                   jae    9a <main+0x61>
  26:   61                      (bad)  
  27:   74 69                   je     92 <main+0x59>
  29:   63 20                   movslq (%rax),%esp
  2b:   66 75 6e                data16 jne 9c <main+0x63>
  2e:   63 74 69 6f             movslq 0x6f(%rcx,%rbp,2),%esi
  32:   6e                      outsb  %ds:(%rsi),(%dx)
  33:   20 63 61                and    %ah,0x61(%rbx)
  36:   6c                      insb   (%dx),%es:(%rdi)
  37:   6c                      insb   (%dx),%es:(%rdi)
  38:   65 64 20 00             gs and %al,%fs:(%rax)
*/


/*
To Understand more about .data  section:
int x=10; // initliazed data section .data , initalized x  global varaible
int y; // unintilized data section .bss , unintialized y global variable

microcontroller:(ROM+RAM)
ROM:(non- volatile memory)(when you loss power_cycle/reset microcontroller, it whould not loss the data)
 - in ROM , .data ,.text section are stored.
 - the elf will have this values of global x=10.
 - when MCU is programmed, this x=10 value which present in elf, will be loaded into ROM of microcontroller,
   so, when power cycle/reset x=10 is not lost and these are some important values which software require to run the system
RAM:(volatile memory)
 - What if X value to be written as x=20, During the Program execution we copy the .data section .text section from ROM to RAM.
 - Then we do modification on .data section in RAM x=20, which will have not effect on the value of x=10 in ROM when we power_cyle/reset.
 
Why .bss section is not stored in ROM (as part of elf)?
 - .bss section contains only in RAM. 
 - .bss section contains unintialzed variables with all zero, 
 - if for example some memorr/frame buffers in Kbs/Mbs which are unintialized variables which we can assume to be zero.
    - then it will waste of memory in ROM.
 - 


 -When power_cycle/resert sections are copied from ROM to RAM.
---------		---------
|	|		|	|
|.data	|		|.data	|	
|.text	|   ------> 	|.text	|
|	|		|	|
| ROM   |		|RAM	|
|	|		|	|
---------		---------

ROM : Contains LOAD address.
RAM : Logical Address
.text contains initilization code which will take care of this.

*/

/*
 hosted environment: OS based machine
 freestanding environment: microcontroller based system.
*/

/*
  .text section

   - where code resides.
   - where this code converted to assembly code.
   - 

*/

/*
  .stack section
   - how much we need to utilize?
   - stack section reside in RAM.
   - we reserve some section in RAM for stack.
   - cpu have stackpointer register which tells/holds memory_address in stack section.
   - sp stackpointer -> points to stack.  
*/


/*
Disassembly of section .bss:

0000000000000000 <stat_uninitvar>:
   0:   00 00                   add    %al,(%rax)
        ...

0000000000000004 <func_stat_uninitvar.2327>:
   4:   00 00                   add    %al,(%rax)
        ...

0000000000000008 <gbl_func_stat_var_unint.2331>:
   8:   00 00                   add    %al,(%rax)
*/

/*
Disassembly of section .data:

0000000000000000 <gbl_initvar>:
   0:   0a 00                   or     (%rax),%al
        ...

0000000000000004 <stat_initvar>:
   4:   14 00                   adc    $0x0,%al
        ...

0000000000000008 <func_stat_initvar.2326>:
   8:   14 00                   adc    $0x0,%al
        ...

000000000000000c <gbl_func_stat_var_int.2330>:
   c:   0a 00                   or     (%rax),%al
        ...
*/
