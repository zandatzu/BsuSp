BITS 32
section .text
	global _start
_start:
    push 0            ; argv[1]: NULL
    push args         ; argv[0]: ptr to "/bin/sh"
    mov ebx, [esp]    ; fname = "/bin/sh"
    mov ecx, esp      ; argv  = ["/bin/sh",NULL]
    mov edx, 0        ; envp  = NULL
 	mov eax, 11       ; syscall 11 (execve)
	int 0x80
section .data
	args db '/bin/sh', 0

