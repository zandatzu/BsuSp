/* Musterlösung zu Aufgabe 14.1: Buffer-Overflow
 * Erstellt von Florian Dittrich
 *
 * Lösung zu Schritt 3 für C
 *
 * Kompilieren mit
 *   gcc touch_hacked.c -o touch_hacked -Wall
 * Ausführen mit
 *   ./touch_hacked
 * Weitere Informationen unter
 *   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k14a1/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char shellcode[] =
#include "touch_hacked.h"

int main(int argc, char *argv[]) {
    // Zuerst den Shellcode in einen Speicherbereich kopieren,
    // in den auch geschrieben werden darf.
    char* mem = malloc(sizeof(shellcode));
    memcpy(mem, shellcode, sizeof(shellcode));

    // Nun den Shellcode ausführen
    void (*shell)() = (void*) mem;
    shell();

    printf("Diese Zeile sollte niemals ausgegeben werden.\n");
}