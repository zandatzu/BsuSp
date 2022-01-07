CC = gcc
CFLAGS = -Wall

run: main
	./main

test: mem_test
	./mem_test

dump: mem_dump
	./mem_dump

main: mem_manager.h mem_manager.c main.c
	$(CC) $(CFLAGS) mem_manager.c main.c -o main

mem_test: mem_manager.h mem_manager.c mem_test.h mem_test.c
	$(CC) $(CFLAGS) mem_test.c -o mem_test -DMEM_MANAGER_FILE='"mem_manager.c"'

mem_dump: mem_manager.h mem_manager.c mem_test.h mem_test.c
	$(CC) $(CFLAGS) mem_test.c -o mem_dump -DMEM_MANAGER_FILE='"mem_manager.c"' -DWRAPPER_DUMP