include ../../Makefile.inc

P=pthreads/part2/
all:
	${GCC} consistent.c -lpthread -o consistent ${ECHO}consistent.c
	${GCC} errchk.c -lpthread -o errchk ${ECHO}errchk.c
	${GCC} recur.c -lpthread -o recur ${ECHO}recur.c

clean:
	${RM} *~ consistent errchk recur
