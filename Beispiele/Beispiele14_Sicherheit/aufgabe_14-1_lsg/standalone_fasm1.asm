; Musterlösung zu Aufgabe 14.1: Buffer-Overflow
; Erstellt von Florian Dittrich
;
; Lösung zu Schritt 1 für FASM
;
; Assemblieren mit
;   fasm standalone.asm
; Ausführen mit
;   ./standalone
; Weitere Informationen unter
;   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k14a1/

format ELF executable 3
segment readable executable
entry start

start:
    mov    eax, 11        ;syscall 11 (execve)
    mov    ebx, filename
    mov    ecx, argv
    mov    edx, 0
    int    0x80

segment readable writeable

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