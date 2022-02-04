CC = cc
CFLAGS = -Wall -c

#
# Variables and rules for building test program
#
OUTPUT = demo

$(OUTPUT): lib.a
	$(CC) $(CFLAGS) testlib.c -o testlib.o
	$(CC) -o $(OUTPUT) testlib.o libhello.a


#
# Variables and rules for building static library
#

# compile and link library
lib.a:
	$(CC) $(CFLAGS) hello.c
	ar -cvq libhello.a hello.o

# cleanup
clean:
	rm libhello*
	rm *.o $(OUTPUT)

# run demo
run:
	./demo
