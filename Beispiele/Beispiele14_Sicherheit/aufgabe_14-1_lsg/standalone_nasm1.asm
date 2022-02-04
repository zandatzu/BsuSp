; Musterlösung zu Aufgabe 14.1: Buffer-Overflow
; Erstellt von Florian Dittrich
;
; lösung zu Schritt 1 für NASM
;
; Assemblieren mit
;   nasm standalone.asm -o standalone
; Ausführen mit
;   ./standalone
; Weitere Informationen unter
;   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k14a1/

BITS 32
section .text
global _start

_start:
    mov    eax, 11        ;syscall 11 (execve)
    mov    ebx, filename
    mov    ecx, argv
    mov    edx, 0
    int    0x80

section .data

filename:               ; Entspricht
    db '/bin/touch', 0  ;   char filename = "/bin/touch";
argv0:
    db 'touch', 0       ;   char argv0 = "touch";
argv1:
    db 'hacked', 0      ;   char argv1 = "hacked";
argv:
    dd argv0            ;   char *argv[] = {argv0,
    dd argv1            ;                   argv1,
    dd 0                ;                   NULL};