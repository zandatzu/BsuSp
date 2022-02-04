/* testlib.c - demonstrate direct use of the "hello" routine */
#include "hello.h"

int main() {
   hello("shared library loaded at loading time.");
   return 0;
}

