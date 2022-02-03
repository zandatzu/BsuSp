/* Musterloesung zu Aufgabe 13.2: Eingabegeräte auslesen
 * Erstellt von Florian Dittrich
 * 
 * Teil (c): Hier soll mit der Maus auf den Framebuffer aus
 *           Aufgabe 13.1 gezeichnet werden.
 *
 * Kompilieren mit
 *   gcc aufgabe_13-2A.c -o aufgabe_13-2A -Wall -std=c99
 * Ausfuehren mit
 *   ./aufgabe_13-2A
 * Weitere Informationen unter
 *   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k13a2
 */

#include <stdio.h>     // printf
#include <stdlib.h>    // exit
#include <stdint.h>    // uint32_t
#include <unistd.h>    // close
#include <fcntl.h>     // open
#include <sys/mman.h>  // mmap
#include <sys/ioctl.h> // ioctl
#include <linux/fb.h>  // struct fb_fix_screeninfo, struct fb_var_screeninfo

#define FBP(x,y) fbp[x + y*(finfo.line_length >> 2)]

int main() {
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;

    // Gerätedatei für Frambuffer öffnen
    int fd = open("/dev/fb0", O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "ERROR: /dev/fb0 konnte nicht geöffnet werden\n");
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) != 0) {
        printf("ERROR: FBIOGET_FSCREENINFO konnte nicht abgefragt werden\n");
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) != 0) {
        printf("ERROR: FBIOGET_FSCREENINFO konnte nicht abgefragt werden\n");
        exit(EXIT_FAILURE);
    }

    // Speicher des Framebuffers einblenden
    uint32_t* fbp = mmap(NULL, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED,
                         fd, 0);
    if (fbp < 0) {
        printf("ERROR: mmap fehlgeschlagen\n");
        exit(EXIT_FAILURE);
    }
    close(fd);

    // Das Makro FBP funktioniert nur bei 32bpp
    if (vinfo.bits_per_pixel != 32)
        exit(EXIT_FAILURE);
	
    // Gesamten Bildschirm schwarz einfärben
    for (int x = 0; x < vinfo.xres; x++)
        for (int y=0; y < vinfo.yres; y++)
            FBP(x, y) = 0;

    // Gerätedatei für Mauszeiger öffnen
    int mice = open("/dev/input/mice", O_RDONLY | O_SYNC);
    if (mice == -1) {
        printf("ERROR: /dev/input/mice konnte nicht geoeffnet werden. Sind Sie root?\n");
        exit(EXIT_FAILURE);
    }

    int x = 0, y = 0;
    char paket[3];
    while (1) {
        // Datenpaket einlesen und Mausposition aktualisieren
        read(fd, paket, 3);
        x += paket[1];
        y -= paket[2];

        // Sicherstellen, dass der Mauszeiger im Bild bleibt
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;
        if (x >= vinfo.xres)
            x = vinfo.xres - 1;
        if (y >= vinfo.yres)
            y = vinfo.yres - 1;


        // Maustasten dekodieren und Pixel einfaerben
        char left   = paket[0] & 1;
        char right  = paket[0] & 2;
        char middle = paket[0] & 4;
        FBP(x, y) = (left   ? 0 : 0xFF0000)
                  ^ (middle ? 0 : 0x00FF00)
                  ^ (right  ? 0 : 0x0000FF);
    }

    close(mice);
    munmap(fbp, finfo.smem_len);
    exit(EXIT_SUCCESS);
}
