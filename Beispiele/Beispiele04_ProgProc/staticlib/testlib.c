/* testlib.c - demonstrate function call to shared library */
#include "hello.h"

int main() {
   hello("static library.");
   return 0;
}

