   	GLOBAL	cmp1
cmp1:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
; [ebp+12] : b
	mov	eax, dword [ebp+12]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
	mov	eax, 1
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : a
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	add	eax, dword [ebp-8]
; [ebp-4] : (temporal variable)
	cmp	eax, dword [ebp-4]
	setg	al
	movzx	eax, al
	cmp	eax, 0
	je	L1_cmp1_if_s2
	mov	eax, 1
	jmp	L0_cmp1_ret
	jmp	L2_cmp1_if_end
L1_cmp1_if_s2:
	mov	eax, 0
	jmp	L0_cmp1_ret
L2_cmp1_if_end:
L0_cmp1_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	cmp2
cmp2:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 1
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+12] : b
	mov	eax, dword [ebp+12]
; [ebp-8] : (temporal variable)
	add	eax, dword [ebp-8]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : a
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	cmp	eax, dword [ebp-4]
	setge	al
	movzx	eax, al
	cmp	eax, 0
	je	L4_cmp2_if_s2
	mov	eax, 1
	jmp	L3_cmp2_ret
	jmp	L5_cmp2_if_end
L4_cmp2_if_s2:
	mov	eax, 0
	jmp	L3_cmp2_ret
L5_cmp2_if_end:
L3_cmp2_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	cmp3
cmp3:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 1
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+12] : b
	mov	eax, dword [ebp+12]
; [ebp-8] : (temporal variable)
	add	eax, dword [ebp-8]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : a
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	cmp	eax, dword [ebp-4]
	setg	al
	movzx	eax, al
	cmp	eax, 0
	je	L7_cmp3_if_s2
	mov	eax, 1
	jmp	L6_cmp3_ret
	jmp	L8_cmp3_if_end
L7_cmp3_if_s2:
	mov	eax, 0
	jmp	L6_cmp3_ret
L8_cmp3_if_end:
L6_cmp3_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 0
	push	eax
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	call	cmp1
	add	esp, 8
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
	mov	eax, 1
	push	eax
	mov	eax, 3
	push	eax
	mov	eax, 4
	push	eax
	call	cmp2
	add	esp, 8
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
	mov	eax, 1
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 4
	push	eax
	call	cmp3
	add	esp, 8
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
L9_main_ret:
	mov	esp, ebp
	pop	ebp
	ret
