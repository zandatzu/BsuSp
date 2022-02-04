#include <stdio.h>
#include <string.h>

extern char *buff;

int main() {
    buff[0] = 'A';
    buff[1] = '\0';
    printf("%s", buff);
    return 0;
}
