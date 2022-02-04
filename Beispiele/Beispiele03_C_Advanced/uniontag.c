#include <stdio.h>
#include <string.h>

enum UnionTags {CHAR, INT, STRING};

struct TaggedUnion {
    enum UnionTags tag;
    
    union Data {
        char ch;
        int i;
        char str[20];
    } value;
};


int main( ) {
    struct TaggedUnion tu;
    
    printf( "sizeof union : %lu\n", sizeof(tu));
    
    tu.tag = CHAR;
    tu.value.ch = 'A';
    
    printf( "content : %c\n", tu.value.ch);

    return 0;
}
