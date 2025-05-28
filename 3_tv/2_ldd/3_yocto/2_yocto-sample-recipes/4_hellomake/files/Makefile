all: hellomake

hellomake: hellomake.o
	${CC} ${LDFLAGS} hellomake.o -o hellomake

hellomake.o : hellomake.c
	${CC} ${CFLAGS} -c hellomake.c 

clean:
	rm -f hellomake

install: all
	mkdir -p ${DESTDIR}${bindir}
	cp -f hellomake ${DESTDIR}${bindir}
	chmod 755 ${DESTDIR}${bindir}/hellomake

.PHONY: all clean install
