CC = gcc
CFLAGS = -Wall -c

#
# Variables and rules for building test program
#
OUTPUT = demo

$(OUTPUT): lib.so 
	$(CC) $(CFLAGS) dlib.c -o testlib.o 
	$(CC) -o $(OUTPUT) testlib.o -ldl


#
# Variables and rules for building library
#

# version numbers of library
MAJ_NR = 1
REL_NR = $(MAJ_NR).0.0

C_LIB_FLAGS = -fPIC -c

SONAME = libhello.so.$(MAJ_NR)
LIB_FLAGS = -shared -Wl,-soname,$(SONAME) -lc

# compile and link library
lib.so:
	$(CC) $(C_LIB_FLAGS) hello.c
	$(CC) $(LIB_FLAGS) -o libhello.so.$(REL_NR) hello.o
	ln -sf libhello.so.$(REL_NR) libhello.so.$(MAJ_NR)	
	ln -sf libhello.so.$(MAJ_NR) libhello.so

# cleanup
clean:
	rm libhello*
	rm *.o $(OUTPUT)

# run demo
run:
	./demo
