	GLOBAL	lab
lab:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 1
; [ebp-4] : e1
	mov	dword [ebp-4], eax
	mov	eax, 0
; [ebp-8] : e2
	mov	dword [ebp-8], eax
; [ebp-4] : e1
	mov	eax, dword [ebp-4]
	cmp	eax, 0
	je	L1_lab_if_s2
; [ebp-8] : e2
	mov	eax, dword [ebp-8]
	cmp	eax, 0
	je	L3_lab_if_s2
	mov	eax, 0
	jmp	L0_lab_ret
	jmp	L4_lab_if_end
L3_lab_if_s2:
	mov	eax, 2
	jmp	L0_lab_ret
L4_lab_if_end:
	jmp	L2_lab_if_end
L1_lab_if_s2:
L2_lab_if_end:
L0_lab_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 2
	push	eax
	call	lab
	add	esp, 0
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 0
L5_main_ret:
	mov	esp, ebp
	pop	ebp
	ret
