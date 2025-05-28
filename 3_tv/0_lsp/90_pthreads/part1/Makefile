include ../../Makefile.inc

P=threads/part1/
all:
	${GCC} attr5.c -lpthread -o attr5 ${ECHO}attr5.c
	${GCC} attr4.c -lpthread -o attr4 ${ECHO}attr4.c
	${GCC} attr3.c -lpthread -o attr3 ${ECHO}attr3.c
	${GCC} attr2.c -lpthread -o attr2${ECHO}attr2.c
	${GCC} attr1.c -lpthread -o attr1 ${ECHO}attr1.c
	${GCC} simple_thread2.c -lpthread -o simple_thread2${ECHO}simple_thread2.c
	${GCC} simple_thread1.c -lpthread -o simple_thread1 ${ECHO}simple_thread1.c
	${GCC} simple_thread.c -lpthread -o simple_thread ${ECHO}simple_thread.c

clean:
	${RM} *~ attr5 attr4 attr3 attr2 attr1 simple_thread simple_thread1  \
	simple_thread2
