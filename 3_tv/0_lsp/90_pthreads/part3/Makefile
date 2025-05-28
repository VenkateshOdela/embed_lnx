include ../../Makefile.inc

P=pthreads/part3/
all:
	${GCC} barrier.c -lpthread -o barrier ${ECHO}barrier.c
	${GCC} proconsync.c -lpthread -o proconsync2 ${ECHO}proconsync.c
	${GCC} pthread_rwlock.c -lpthread -o pthread_rwlock ${ECHO}pthread_rwlock.c
	${GCC} pthread_spinlock.c -lpthread -o pthread_spinlock ${ECHO}pthread_spinlock.c

clean:
	${RM} *~ barrier prod_condvar2 pthread_spinlock  pthread_rwlock 
