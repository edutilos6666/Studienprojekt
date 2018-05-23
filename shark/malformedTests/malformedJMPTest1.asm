.global maintest
.text

main:	mov #1, r1
	mov #2, r2
	jmp add1
add1:	add r2, r1
	jmp add3
add2:	add r1, r2 
	ret
add3:	add r2, r1
	ret
