#include <sys/mman.h>
#include <unistd.h>
#include "secure_malloc.h"
#include <stdio.h>

int main() {
    int* x = (int*) secure_malloc(sizeof(int));
    *x = 24;
    printf("%d\n",*x);
    printf("%d\n",*(x-getpagesize()+1));
    return 0;
}

void* secure_malloc(size_t nbytes) {
    size_t page_size = (size_t) getpagesize();
    size_t aufgerundet = nbytes + (page_size - nbytes % page_size);
    size_t gesamt_speicher = aufgerundet + 2*page_size;
    void* addresse = mmap(NULL,gesamt_speicher,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,0,0);
    *((size_t*)addresse) = gesamt_speicher;
    mprotect(addresse,page_size,PROT_NONE);
    mprotect(addresse+gesamt_speicher-page_size,page_size,PROT_NONE);
    return addresse+page_size;
}