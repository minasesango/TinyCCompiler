	GLOBAL	w
w:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 0
; [ebp-4] : r
	mov	dword [ebp-4], eax
L1_w_while_begin:
	mov	eax, 0
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	cmp	eax, dword [ebp-8]
	setg	al
	movzx	eax, al
	cmp	eax, 0
	je	L2_w_while_end
	mov	eax, 1
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp-4] : r
	mov	eax, dword [ebp-4]
; [ebp-8] : (temporal variable)
	add	eax, dword [ebp-8]
; [ebp-4] : r
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
	jmp	L1_w_while_begin
L2_w_while_end:
; [ebp-4] : r
	mov	eax, dword [ebp-4]
	jmp	L0_w_ret
L0_w_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 10
	push	eax
	mov	eax, 10
	push	eax
	call	w
	add	esp, 4
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 4
L3_main_ret:
	mov	esp, ebp
	pop	ebp
	ret
