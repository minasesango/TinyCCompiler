   	GLOBAL	l
l:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
; [ebp+16] : z
	mov	eax, dword [ebp+16]
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+12] : y
	mov	eax, dword [ebp+12]
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	add	eax, dword [ebp-8]
; [ebp-4] : (temporal variable)
	add	eax, dword [ebp-4]
	jmp	L0_l_ret
L0_l_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	r
r:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 1
	push	eax
	call	l
	add	esp, 12
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	add	eax, dword [ebp-4]
	jmp	L1_r_ret
L1_r_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	rsl1
rsl1:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 1
	push	eax
	call	l
	add	esp, 12
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-4] : (temporal variable)
	sub	eax, dword [ebp-4]
	jmp	L2_rsl1_ret
L2_rsl1_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	rsl2
rsl2:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 4
	push	eax
	call	r
	add	esp, 4
; [ebp-4] : (temporal variable)
	mov	dword [ebp-4], eax
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 1
	push	eax
	call	l
	add	esp, 12
; [ebp-4] : (temporal variable)
	sub	eax, dword [ebp-4]
	jmp	L3_rsl2_ret
L3_rsl2_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	arth
arth:
	push	ebp
	mov	ebp, esp
	sub	esp, 12
	mov	eax, 10
; [ebp-4] : i
	mov	dword [ebp-4], eax
; [ebp+12] : y
	mov	eax, dword [ebp+12]
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	sub	eax, dword [ebp-8]
; [ebp+8] : x
	mov	dword [ebp+8], eax
	mov	eax, 1
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-12] : (temporal variable)
	mov	dword [ebp-12], eax
; [ebp-4] : i
	mov	eax, dword [ebp-4]
; [ebp-12] : (temporal variable)
	sub	eax, dword [ebp-12]
; [ebp-8] : (temporal variable)
	sub	eax, dword [ebp-8]
; [ebp-4] : i
	mov	dword [ebp-4], eax
; [ebp-4] : i
	mov	eax, dword [ebp-4]
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
; [ebp+8] : x
	mov	eax, dword [ebp+8]
; [ebp-8] : (temporal variable)
	add	eax, dword [ebp-8]
	jmp	L4_arth_ret
L4_arth_ret:
	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:
	push	ebp
	mov	ebp, esp
	sub	esp, 12
	mov	eax, 6
	push	eax
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 1
	push	eax
	call	l
	add	esp, 12
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 12
	mov	eax, 16
	push	eax
	mov	eax, 10
	push	eax
	call	r
	add	esp, 4
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 4
	mov	eax, 4
	push	eax
	mov	eax, 10
	push	eax
	call	rsl1
	add	esp, 4
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 4
; [ebp-8] : (temporal variable)
	mov	dword [ebp-8], eax
	mov	eax, 4
; [ebp-8] : (temporal variable)
	sub	eax, dword [ebp-8]
	push	eax
	call	rsl2
	add	esp, 0
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 0
	mov	eax, 9
	push	eax
	mov	eax, 4
	push	eax
	mov	eax, 2
	push	eax
	call	arth
	add	esp, 8
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
L5_main_ret:
	mov	esp, ebp
	pop	ebp
	ret
