/* Musterloesung zu Aufgabe 13.1: Framebuffer
 * Erstellt von Florian Dittrich
 * 
 * Teil (a): Hier sollen Aufloesung, Farbtiefe und Zeilenlaenge
 *           des Framebuffers ausgelesen und ausgegeben werden.
 *
 * Kompilieren mit
 *   gcc aufgabe_13-1A.c -o aufgabe_13-1A -Wall -std=c99
 * Ausfuehren mit
 *   ./aufgabe_13-1A
 * Weitere Informationen unter
 *   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k13a1
 */

#include <stdio.h>     // Fuer printf
#include <stdlib.h>    // Fuer exit
#include <stdint.h>    // Fuer uint32_t
#include <unistd.h>    // Fuer close
#include <fcntl.h>     // Fuer open
#include <sys/mman.h>  // Fuer mmap
#include <sys/ioctl.h> // Fuer ioctl
#include <linux/fb.h>  // Fuer fb_fix_screeninfo, fb_var_screeninfo

int main() {
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;

    // Framebuffer-Geraetedatei oeffnen
    int fd = open("~/dev/fb0", O_RDWR);
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

    // Gesuchte Werte ausgeben
    printf("Aufloesung:   %ux%upx\n"
           "Farbtiefe:    %u BPP\n"
           "Zeilenlaenge: %u Bytes\n",
           vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.line_length);

    close(fd);
    exit(EXIT_SUCCESS);
}
