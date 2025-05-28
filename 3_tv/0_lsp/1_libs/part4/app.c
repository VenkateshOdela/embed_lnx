#include <stdio.h>
#include "add.h"

int main()
{

 int a=30, b=20;

 add(a,b);
 return 0;
}


/*

 1. Creating Static Library 

   $ gcc -c add.c -o add.o

   $ ar rcs -o libadd.a add.o


 2. Linking static library(libadd.a) with application

   $ gcc -c app.c -o app.o
   
   $ gcc app.o -L. -ladd -o appstatic

 3. ./appstatic

note : runtime libadd.a not necessarly required to be present while executing application ./app
       Because Static library is part of exe.
       - even deleting static library libadd.a, Application ./app work properly; 

gcc flags :

 -L : indicates the location(path) of the library you want to reference

 -l : specify the specific library you want to use

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/libs/part4$ file appstatic 
appstatic: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=4f51afd7e05e16215fcc017b167214e6997bf419, not stripped

*/


/*
 nm - list symbols from object files

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/libs/part4$ nm libadd.a 

add.o:
0000000000000000 T add
                 U _GLOBAL_OFFSET_TABLE_
                 U printf

*/

/*
      ar - create, modify, and extract from archives

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/libs/part4$ ar -t libadd.a 
add.o


*/

/*

 1. Creating Dynamic Library 

   $ gcc -c add.c -o add.o

   $ gcc -shared -o libaddso.so add.o

 2. Linking Dynamic library(libadd.so) with application

   $ gcc -c app.c -o app.o
   
   $ gcc app.o -L. -laddso -o appdynamic

 3. ./appdynamic

note : runtime libaddso.so necessarly required to be present while executing application ./appdynamic
       Because Static library is part of exe.



*/

/*

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/libs/part4$ ./appdynamic 
./appdynamic: error while loading shared libraries: libaddso.so: cannot open shared object file: No such file or directory

export LD_LIBRARY_PATH=/home/user/git_embd_linux/embd_linux/tv/lsp/libs/part4:$LD_LIBRARY_PATH
*/
