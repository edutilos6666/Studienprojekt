.global malformedTest1
.text

main:
	mov #4, r3
	mov #5, r4
	add 1(r3), r4
	ret
