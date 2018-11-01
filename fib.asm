	GLOBAL	fib
fib:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 2
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : n
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	cmp	eax, dword [ebp-4]
	setl	al
	movzx	eax, al
	cmp	eax, 0
	je	L1_fib_if_s2
; [ebp+8] : n
	mov	eax, dword [ebp+8]
	jmp	L0_fib_ret
	jmp	L2_fib_if_end
L1_fib_if_s2:
	mov	eax, 2
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : n
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	sub	eax, dword [ebp-8]
	push	eax
	call	fib
	add	esp, 4
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
	mov	eax, 1
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : n
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	sub	eax, dword [ebp-8]
	push	eax
	call	fib
	add	esp, 4
; [ebp-4] : (temporal variable)
	add	eax, dword [ebp-4]
	jmp	L0_fib_ret
L2_fib_if_end:
L0_fib_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 6765
	push	eax
	mov	eax, 20
	push	eax
	call	fib
	add	esp, 4
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 4
L3_main_ret:
	mov	esp, ebp
	pop	ebp
	ret
