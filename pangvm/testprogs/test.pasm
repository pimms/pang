	jmp		%main

fib:
	mov		ra $10
	jmp		%L0

main:
	jmp 	%fib
L0:
	mov		rb ra
	
