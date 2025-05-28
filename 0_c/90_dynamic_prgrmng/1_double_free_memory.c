#include <stdio.h>
#include <stdlib.h>

#if 0
int main()
{
	int *ptr = malloc(10);

	//ptr++;

	free(ptr);

	free(ptr);

}
#endif

/* solution */
#if 1
int main()
{
	int *ptr = malloc(10);

	//ptr++;

	free(ptr);
    printf("%p \n",ptr);
    //ptr = NULL;
    printf("%p \n",ptr);

	free(ptr);

}
#endif

/*
 * user@ubuntu:~/git_embd_linux/embd_linux/0_c/9_dynmc_prgrmng$ ./a.out
		free(): double free detected in tcache 2
		Aborted (core dumped)
 *

 */

/*
 * user@ubuntu:~/git_embd_linux/embd_linux/0_c/9_dynmc_prgrmng$ valgrind ./a.out
		==73613== Memcheck, a memory error detector
		==73613== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
		==73613== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
		==73613== Command: ./a.out
		==73613==
		==73613== Invalid free() / delete / delete[] / realloc()
		==73613==    at 0x4C32D3B: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
		==73613==    by 0x1086B7: main (in /home/user/git_embd_linux/embd_linux/0_c/9_dynmc_prgrmng/a.out)
		==73613==  Address 0x522f040 is 0 bytes inside a block of size 10 free'd
		==73613==    at 0x4C32D3B: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
		==73613==    by 0x1086AB: main (in /home/user/git_embd_linux/embd_linux/0_c/9_dynmc_prgrmng/a.out)
		==73613==  Block was alloc'd at
		==73613==    at 0x4C31B0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
		==73613==    by 0x10869B: main (in /home/user/git_embd_linux/embd_linux/0_c/9_dynmc_prgrmng/a.out)
		==73613==
		==73613==
		==73613== HEAP SUMMARY:
		==73613==     in use at exit: 0 bytes in 0 blocks
		==73613==   total heap usage: 1 allocs, 2 frees, 10 bytes allocated
		==73613==
		==73613== All heap blocks were freed -- no leaks are possible
		==73613==
		==73613== For counts of detected and suppressed errors, rerun with: -v
		==73613== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
 *
 */
