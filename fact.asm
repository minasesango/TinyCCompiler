	GLOBAL	fact
fact:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 1
; [ebp-4] : z
	mov	dword [ebp-4], eax
L1_fact_while_begin:
	mov	eax, 1
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	cmp	eax, dword [ebp-8]
	setge	al
	movzx	eax, al
	cmp	eax, 0
	je	L2_fact_while_end
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp-4] : z
	mov	eax, dword [ebp-4]
; [ebp-8] : (temporal variable)
	imul	eax, dword [ebp-8]
; [ebp-4] : z
	mov	dword [ebp-4], eax
	mov	eax, 1
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	sub	eax, dword [ebp-8]
; [ebp+8] : x
	mov	dword [ebp+8], eax
	jmp	L1_fact_while_begin
L2_fact_while_end:
; [ebp-4] : z
	mov	eax, dword [ebp-4]
	jmp	L0_fact_ret
L0_fact_ret:
	mov	esp, ebp
	pop	ebp
	ret
