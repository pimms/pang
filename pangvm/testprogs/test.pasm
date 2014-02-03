	; Move 16 to RA
	mov		ra 	$16

	; Move 16 ro RB
	mov 	rb  ra

	; Push RB 
	push	rb

	; Assign 42 to RB
	mov 	rb  $42

	; Move 42 to RA
	mov 	ra rb

	; Pop 16 back into RB
	pop		rb

	; Move 0 to RA and RB
	mov		ra $0
	mov		rb $0
