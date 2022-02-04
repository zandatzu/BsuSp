#include <stdio.h>
struct datum {
    int tag;
    int monat;
    int jahr;
};
typedef struct datum datum_t;

void change(datum_t *dat) {
    dat->jahr = 1900;
}

void no_change(datum_t dat) {
    dat.jahr = 1900;
}

int main() {
    datum_t  d1, d2;
    datum_t *ptr = &d1;
    
    change( ptr );
    printf("Jahr = %u\n", d1.jahr);
    
    no_change( d2 );
    printf("Jahr = %u\n", d2.jahr);
    
    return 0;
}
