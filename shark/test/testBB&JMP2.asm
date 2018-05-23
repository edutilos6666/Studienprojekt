.global maintest
.text

main:	
	mov #1, r1
	mov #2, r2
	jmp subz
subz:
	sub r2, r1
	jmp addz
addz:	
	add r1, r2
	jmp more
more:
	mov #3, r3
	jmp moree
moree:
	mov r1, r2
 	jmp moreee
moreee:
	mov r2, r1
	jmp moreeee
moreeee:
	mov r1, r2
	ret

