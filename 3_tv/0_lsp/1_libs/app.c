#include <stdio.h>

int main()
{
   printf("venkatesh odela\n");
   return 0;
}


/* 
dynamic linking :

gcc app.c -o appdyn

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/libs$ file appdyn 
appdyn: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=22a1f26610a255d20261b6cd197af05e8e34ac83, not stripped

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/libs$ du appdyn -h
12K	appdyn


*/


/*
static linking

gcc -static app.c -o appstatic

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/libs$ file appstatic 
appstatic: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, for GNU/Linux 3.2.0, BuildID[sha1]=5621f04d06b8ca2b2801f4c620ebeabd0047ce86, not stripped

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/libs$ du appstatic -h
828K	appstatic


*/


/*
  In static linking, Size of excutable is more : 828K

  In Dynamic linking, Size of excutable is small : 12K
*/
