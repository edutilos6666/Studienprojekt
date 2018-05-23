.global maintest
.text

main:	mov #1, r1
	mov #2, r2
	jmp subz
subz:	sub r2, r1
	jmp addz
addz:	add r1, r2 
	ret

