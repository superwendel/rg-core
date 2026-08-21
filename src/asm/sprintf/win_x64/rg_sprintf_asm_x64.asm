; rg_sprintf x64 asm helpers

OPTION CASEMAP:NONE

PUBLIC rg_strlen_asm
PUBLIC rg_utoa_asm
PUBLIC rg_u64toa_asm

.code

rg_strlen_asm PROC
	mov rax, rcx
	vpxor ymm0, ymm0, ymm0
	; Page-safe AVX2 scan:
	; - vector scan while the 32-byte load stays inside the current 4 KiB page
	; - scalar scan the tail near page end to avoid cross-page faults
	mov r8, rax
	and r8, 0FFFFFFFFFFFFF000h   ; current page base
	add r8, 1000h                ; start of next page
	lea r9, [r8 - 32]            ; last safe 32-byte load start in this page

ALIGN 16
rg_strlen_loop:
	cmp rax, r9
	ja rg_strlen_tail

	vmovdqu ymm1, ymmword ptr [rax]
	vpcmpeqb ymm1, ymm1, ymm0
	vpmovmskb edx, ymm1
	test edx, edx
	jnz rg_strlen_found
	add rax, 32
	jmp rg_strlen_loop

rg_strlen_tail:
	cmp rax, r8
	jae rg_strlen_next_page
	cmp byte ptr [rax], 0
	je rg_strlen_found_byte
	inc rax
	jmp rg_strlen_tail

rg_strlen_next_page:
	add r8, 1000h
	lea r9, [r8 - 32]
	jmp rg_strlen_loop

rg_strlen_found:
	bsf edx, edx
	add rax, rdx

rg_strlen_found_byte:
	sub rax, rcx
	vzeroupper
	ret
rg_strlen_asm ENDP

; char* rg_utoa_asm(uint32_t value, char* buf, int digits, const char* digit_pairs)
rg_utoa_asm PROC
	; rcx = value
	; rdx = buf
	; r8d = digits
	; r9  = digit_pairs
	mov eax, ecx
	lea r10, [rdx + r8]
	mov r11, r10
	mov byte ptr [r10], 0

rg_utoa_loop4:
	cmp eax, 10000
	jb rg_utoa_loop2

	mov r8d, eax
	mov ecx, 0D1B71759h
	mul ecx
	mov ecx, edx
	shr ecx, 13

	imul edx, ecx, 10000
	sub r8d, edx

	sub r10, 4

	mov eax, r8d
	imul rax, rax, 051EB851Fh
	shr rax, 37
	mov edx, eax
	imul eax, edx, 100
	sub r8d, eax

	lea eax, [rdx*2]
	mov ax, word ptr [r9 + rax]
	mov word ptr [r10], ax
	lea eax, [r8d*2]
	mov ax, word ptr [r9 + rax]
	mov word ptr [r10 + 2], ax

	mov eax, ecx
	jmp rg_utoa_loop4

rg_utoa_loop2:
	cmp eax, 100
	jb rg_utoa_tail

	mov ecx, eax
	imul rax, rax, 051EB851Fh
	shr rax, 37
	mov edx, eax
	imul eax, edx, 100
	sub ecx, eax

	sub r10, 2
	lea eax, [rcx*2]
	mov ax, word ptr [r9 + rax]
	mov word ptr [r10], ax

	mov eax, edx
	jmp rg_utoa_loop2

rg_utoa_tail:
	cmp eax, 10
	jb rg_utoa_one
	sub r10, 2
	lea eax, [rax*2]
	mov ax, word ptr [r9 + rax]
	mov word ptr [r10], ax
	jmp rg_utoa_done

rg_utoa_one:
	sub r10, 1
	add al, '0'
	mov byte ptr [r10], al

rg_utoa_done:
	mov rax, r11
	ret
rg_utoa_asm ENDP

; char* rg_u64toa_asm(uint64_t value, char* buf, int digits, const char* digit_quads)
rg_u64toa_asm PROC
	push r12
	push r13
	push r14
	push r15

	mov rax, rcx
	lea r10, [rdx + r8]
	mov r11, r10
	mov byte ptr [r10], 0

rg_u64_loop:
	cmp rax, 1000000000
	jb rg_u64_tail

	xor rdx, rdx
	mov r8, 1000000000
	div r8
	mov r15, rax
	mov r12d, edx
	sub r10, 9
	lea r14, [r10 + 1]

	mov eax, r12d
	mov r8d, 02AF31DC5h
	mul r8d
	mov ecx, edx
	shr ecx, 24
	imul edx, ecx, 100000000
	mov r13d, r12d
	sub r13d, edx

	add cl, '0'
	mov byte ptr [r10], cl

	mov eax, r13d
	mov r8d, 0D1B71759h
	mul r8d
	mov eax, edx
	shr eax, 13
	mov r12d, eax
	imul edx, r12d, 10000
	mov ecx, r13d
	sub ecx, edx

	mov eax, r12d
	lea edx, [rax*4]
	mov eax, dword ptr [r9 + rdx]
	mov dword ptr [r14], eax
	mov eax, ecx
	lea edx, [rax*4]
	mov eax, dword ptr [r9 + rdx]
	mov dword ptr [r14 + 4], eax

	mov rax, r15
	jmp rg_u64_loop

rg_u64_tail:
rg_u64_tail_loop:
	cmp eax, 100
	jb rg_u64_tail_small

	mov r8d, eax
	mov ecx, 051EB851Fh
	mul ecx
	mov ecx, edx
	shr ecx, 5
	imul edx, ecx, 100
	sub r8d, edx

	sub r10, 2
	lea edx, [r8d*4 + 2]
	mov ax, word ptr [r9 + rdx]
	mov word ptr [r10], ax

	mov eax, ecx
	jmp rg_u64_tail_loop

rg_u64_tail_small:
	cmp eax, 10
	jb rg_u64_tail_one
	sub r10, 2
	lea edx, [rax*4 + 2]
	mov ax, word ptr [r9 + rdx]
	mov word ptr [r10], ax
	jmp rg_u64_done

rg_u64_tail_one:
	sub r10, 1
	add al, '0'
	mov byte ptr [r10], al

rg_u64_done:
	mov rax, r11
	pop r15
	pop r14
	pop r13
	pop r12
	ret
rg_u64toa_asm ENDP

END
