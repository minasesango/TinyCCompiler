 	GLOBAL	gcd
gcd:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
; [ebp+12] : b
	mov	eax, dword [ebp+12]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : a
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	cmp	eax, dword [ebp-4]
	sete	al
	movzx	eax, al
	cmp	eax, 0
	je	L1_gcd_if_s2
; [ebp+8] : a
	mov	eax, dword [ebp+8]
	jmp	L0_gcd_ret
	jmp	L2_gcd_if_end
L1_gcd_if_s2:
; [ebp+12] : b
	mov	eax, dword [ebp+12]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : a
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	cmp	eax, dword [ebp-4]
	setg	al
	movzx	eax, al
	cmp	eax, 0
	je	L3_gcd_if_s2
; [ebp+12] : b
	mov	eax, dword [ebp+12]
	push	eax
; [ebp+12] : b
	mov	eax, dword [ebp+12]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : a
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	sub	eax, dword [ebp-4]
	push	eax
	call	gcd
	add	esp, 8
	jmp	L0_gcd_ret
	jmp	L4_gcd_if_end
L3_gcd_if_s2:
; [ebp+8] : a
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+12] : b
	mov	eax, dword [ebp+12]
; [ebp-4] : (temporal variable)
	sub	eax, dword [ebp-4]
	push	eax
; [ebp+8] : a
	mov	eax, dword [ebp+8]
	push	eax
	call	gcd
	add	esp, 8
	jmp	L0_gcd_ret
L4_gcd_if_end:
L2_gcd_if_end:
L0_gcd_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 63
	push	eax
	mov	eax, 189
	push	eax
	mov	eax, 315
	push	eax
	call	gcd
	add	esp, 8
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
L5_main_ret:
	mov	esp, ebp
	pop	ebp
	ret
