/* Musterloesung zu Aufgabe 13.2: Eingabegeräte auslesen
 * Erstellt von Florian Dittrich
 * 
 * Teil (b): Hier sollen absolute Mauskoordinaten ausgegeben werden.
 *
 * Kompilieren mit
 *   gcc aufgabe_13-2B.c -o aufgabe_13-2B -Wall -std=c99
 * Ausfuehren mit
 *   ./aufgabe_13-2B
 * Weitere Informationen unter
 *   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k13a2
 */

#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <unistd.h> // close
#include <fcntl.h>  // open

int main() {
    // Gerätedatei öffnen
    int fd = open("/dev/input/mice", O_RDONLY | O_SYNC);
    if (fd == -1) {
        printf("ERROR: /dev/input/mice konnte nicht geoeffnet werden. "
               "Sind Sie root?\n");
        exit(EXIT_FAILURE);
    }

    int x = 0, y = 0;
    char paket[3];
    while (1) {
        // Datenpaket einlesen und Mausposition aktualisieren
        read(fd, paket, 3);
        x += paket[1];
        y += paket[2];

        // Status der Maustasten dekodieren und ausgeben
        char left   = (paket[0] & 1)?'L':' ';
        char right  = (paket[0] & 2)?'R':' ';
        char middle = (paket[0] & 4)?'M':' ';
        printf("%c%c%c Delta-X: %2d, Delta-Y: %2d, X: %4d, Y: %4d\n",
               left, right, middle, paket[1], paket[2], x, y);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
