.global malformedTest2
.text

main:
	mov #4, r3
	mov #7, r4
	add @r3, r4
	ret
