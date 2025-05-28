/*
 * strcat, strncat - concatenate two strings
 *
 */

#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 512
#define FILE_NAME_TEXT "io-server_status_file: "

#define IP "21.36,09212022 11:54:24,start,2249,-"

char cWorkBuffer[BLOCK_SIZE];

int main()
{
	/*
 char s1[512];
 memset(s1, 0xaa, sizeof(s1));
 char s2[512]="odsfj";

 strncat(s1 ,s2, sizeof(s1));

 printf("s1 :%s\n",s1); */

 char temp[BLOCK_SIZE];

 memset(temp, 0x00, sizeof(temp));

 strncpy(temp, FILE_NAME_TEXT, sizeof(temp));

 printf("string length :%d",strlen(temp));

 //memset(temp, 0x22, sizeof(temp));

 //memset(cWorkBuffer, 0x22, sizeof(cWorkBuffer));

 strncpy(cWorkBuffer, IP, sizeof(cWorkBuffer));

 //strncat(temp, cWorkBuffer, sizeof(temp));

 strncat(temp, cWorkBuffer, strlen(cWorkBuffer));

 printf("temp :%s\n",temp);




}
/*
int main()
{
 char s1[2] = "ve";
 char s2[2] = "od";

 strncat(s1 ,s2, 2);

 printf("s1 :%s\n",s1);

}*/
/*
 * user@ubuntu:~/git_embd_linux/embd_linux/c/strings$ ./a.out s1 :veodod
*** stack smashing detected ***: <unknown> terminated
Aborted (core dumped)
 */
/*
 * user@ubuntu:~/git_embd_linux/embd_linux/c/strings$ valgrind ./a.out
==84726== Memcheck, a memory error detector
==84726== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==84726== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==84726== Command: ./a.out
==84726==
==84726== Source and destination overlap in strncat(0x1ffefffb44, 0x1ffefffb46, 2)
==84726==    at 0x4C34C73: strncat (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==84726==    by 0x108734: main (in /home/user/git_embd_linux/embd_linux/c/strings/a.out)
==84726==
s1 :veodod
*** stack smashing detected ***: <unknown> terminated
==84726==
==84726== Process terminating with default action of signal 6 (SIGABRT)
==84726==    at 0x4E7CE87: raise (raise.c:51)
==84726==    by 0x4E7E7F0: abort (abort.c:79)
==84726==    by 0x4EC7836: __libc_message (libc_fatal.c:181)
==84726==    by 0x4F72B30: __fortify_fail_abort (fortify_fail.c:33)
==84726==    by 0x4F72AF1: __stack_chk_fail (stack_chk_fail.c:29)
==84726==    by 0x108765: main (in /home/user/git_embd_linux/embd_linux/c/strings/a.out)
==84726==
==84726== HEAP SUMMARY:
==84726==     in use at exit: 0 bytes in 0 blocks
==84726==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
==84726==
==84726== All heap blocks were freed -- no leaks are possible
==84726==
==84726== For counts of detected and suppressed errors, rerun with: -v
==84726== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
Aborted (core dumped
 */
