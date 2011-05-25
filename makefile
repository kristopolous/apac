GCC=gcc -g3 -gdwarf-2 -Wall -DUSECGI -D_DEBUG
os=$(shell uname)
LD=
# Solaris is special!
ifeq ($(os),SunOS)
	CC=$(GCC) -D_$(os)
	LD=-lsocket
else
	CC=$(GCC) -D_$(os)
endif
ap: server.o magic.o parse.o defines.o
	$(CC) -c apac.c
	$(CC) -o apac *.o $(LD)
install:
	cp ext.mime ~/.apac.mime
	cp apac /usr/local/bin
package:
	echo $(MYOS)
clean:
	rm -f core apac *.o
