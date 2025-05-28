
#include <stdio.h>

void calculate(int a) {
  if (a == 0)
    return;

      a = a+1;
      printf("%d\n",a);
      calculate(a);
}
int main() {
   int a = 1;
   calculate(a);
}

/*
If a function is called recursively by itself infinite times
then stack will be unable to store large number of local variables,
so stack overflow will occur − */



/*
to see stack overflow with valgrind tool

$valgrind ./a.out
==69907== Stack overflow in thread #1: can't grow stack to 0x1ffe801000
261798
==69907== Stack overflow in thread #1: can't grow stack to 0x1ffe801000
261799
==69907== Stack overflow in thread #1: can't grow stack to 0x1ffe801000
261800
==69907== Stack overflow in thread #1: can't grow stack to 0x1ffe801000
261801
==69907== Stack overflow in thread #1: can't grow stack to 0x1ffe801000
==69907== 



*/
