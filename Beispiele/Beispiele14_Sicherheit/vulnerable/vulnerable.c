/* 
   compile:

   gcc -fno-stack-protector -m32 -o vulnerable vulnerable.c 

   -fno-stack-protector -> disable canary
*/


#include <stdio.h>
#include <string.h>

void normal(char *string) {
   char buf[4];

   strcpy(buf,string);
   printf("(%i) %s\n", strlen(string), buf);
}

int main(int argc, char** argv) {
   if (argc==2) normal(argv[1]);
   return -1;
}
