	COMMON	x 4
	COMMON	y 4
	GLOBAL	f
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 3
	mov	[y], eax
	mov	eax, [y]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	add	eax, dword [ebp-4]
	jmp	L0_f_ret
L0_f_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	g
g:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
; [ebp+8] : y
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
	mov	eax, [x]
; [ebp-4] : (temporal variable)
	add	eax, dword [ebp-4]
	jmp	L1_g_ret
L1_g_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 4
	mov	[x], eax
	mov	eax, 5
	push	eax
	mov	eax, 2
	push	eax
	call	f
	add	esp, 4
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 4
	mov	eax, 7
	push	eax
	mov	eax, 3
	push	eax
	call	g
	add	esp, 4
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 4
L2_main_ret:
	mov	esp, ebp
	pop	ebp
	ret
