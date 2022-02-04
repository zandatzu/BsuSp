/* Musterlösung zu Aufgabe 14.1: Buffer-Overflow
 * Erstellt von Florian Dittrich
 *
 * Lösung zu Schritt 1 für C
 *
 * Kompilieren mit
 *   gcc standalone.c -o standalone -Wall
 * Ausführen mit
 *   ./standalone
 * Weitere Informationen unter
 *   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k14a1/
 */

#include <unistd.h>

int main() {
        char *argv[] = {"touch", "hacked", NULL};
        execve("/bin/touch", argv, NULL);
}