/* 
   compile:

   gcc -fno-stack-protector -m32 -o gets gets.c 

   -fno-stack-protector -> disable canary
*/


#include <stdio.h>
#include <string.h>


char * vulnFunction() {
   char buffer[128];
 
   /* read string from standard input */
   gets( buffer );

   /* return ptr. to buffer, copied to heap */
   return strdup( buffer );
}




int main(int argc, char** argv) {
   vulnFunction();
}
