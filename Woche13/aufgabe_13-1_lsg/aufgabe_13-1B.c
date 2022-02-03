/* Musterloesung zu Aufgabe 13.1: Framebuffer
 * Erstellt von Florian Dittrich
 * 
 * Teil (b): Hier soll der Framebuffer eingefaerbt werden.
 *
 * Kompilieren mit
 *   gcc aufgabe_13-1B.c -o aufgabe_13-1B -Wall -std=c99
 * Ausfuehren mit
 *   ./aufgabe_13-1B
 * Weitere Informationen unter
 *   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k13a1
 */

#include <stdio.h>     // Fuer printf
#include <stdlib.h>    // Fuer exit
#include <stdint.h>    // Fuer uint32_t
#include <unistd.h>    // Fuer close
#include <fcntl.h>     // Fuer open
#include <string.h>    // Fuer memset
#include <sys/mman.h>  // Fuer mmap
#include <sys/ioctl.h> // Fuer ioctl
#include <linux/fb.h>  // Fuer fb_fix_screeninfo, fb_var_screeninfo

#define FBP(x,y) fbp[x + y*(finfo.line_length >> 2)]

int main() {
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;

    // Framebuffer-Geraetedatei oeffnen
    int fd = open("/dev/fb0", O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "ERROR: /dev/fb0 konnte nicht geöffnet werden\n");
        exit(EXIT_FAILURE);
    }

    // Datenstrukturen befuellen lassen
    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) != 0) {
        printf("ERROR: FBIOGET_FSCREENINFO konnte nicht abgefragt werden\n");
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) != 0) {
        printf("ERROR: FBIOGET_VSCREENINFO konnte nicht abgefragt werden\n");
        exit(EXIT_FAILURE);
    }

    // Den Framebuffer in den virtuellen Speicher des Programms einblenden
    uint32_t* fbp = mmap(NULL, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fbp == NULL) {
        printf("ERROR: mmap fehlgeschlagen\n");
        exit(EXIT_FAILURE);
    }
    close(fd);

    // Das Makro FBP funktioniert nur bei 32bpp, daher wird hier unterschieden:
    if (vinfo.bits_per_pixel == 32) {
        // Gesamten Bildschirm gelb einfärben
        for (int x = 0; x < vinfo.xres; x++)
            for (int y=0; y < vinfo.yres; y++)
                FBP(x, y) = 0xFFFF00;

	} else {
        // Ansonsten den Bildschirm grau einfaerben
        memset(fbp, 0xCC, finfo.smem_len);
    }

    munmap(fbp, finfo.smem_len);
    exit(EXIT_SUCCESS);
}
