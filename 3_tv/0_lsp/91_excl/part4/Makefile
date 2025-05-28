CC=gcc
CFLAGS= -c
INCLUDE= -I./
LIBS= -lpthread

App:thr_shdata.o excl.o
	$(CC) thr_shdata.o excl.o $(LIBS) -o App
	echo "Application Image Ready"

thr_shdata.o: thr_shdata.c
	$(CC) $(INCLUDE) $(CFLAGS) thr_shdata.c 
	echo "thr_shdata.o generated"

excl.o: excl.c
	$(CC) $(INCLUDE) $(CFLAGS) excl.c
	echo "excl.o generated"

clean:
	rm -rf *.o App a.out
	

