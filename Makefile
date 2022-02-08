all: logappend logread

CFLAGS=-g 

# Add any libraries you want to include here via: -lthe_library_name
LFLAGS=

# Add source code files into the list
SRC=brg_types.h data.c data.h logappend.c logread.c Makefile prepSrcCrypto.sh postSrcCrypto.sh

logappend: logappend.o data.o
	./prepSrcCrypto.sh
	$(CC) -g -o logappend logappend.o data.o $(LFLAGS)

logread: logread.o data.o
	$(CC) $(CFLAGS) -o logread logread.o data.o $(LFLAGS)
	./postSrcCrypto.sh
	
logappend.o: logappend.c
	$(CC) $(CFLAGS) -c -o logappend.o logappend.c

logread.o: logread.c
	$(CC) $(CFLAGS) -c -o logread.o logread.c

data.o: data.c
	$(CC) $(CFLAGS) -c -o data.o data.c

handin: clean
	rm -f handin.tar
	tar -cvf handin.tar --exclude=handin.tar $(SRC)

clean:
	rm -f *.o
	rm -f logappend logread
