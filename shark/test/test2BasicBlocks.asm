.global test2BasicBlocks
.text

main:
	mov #2, r1
	mov #4, r2
	add r1, r2
	ret

testlabel:
	mov #5, r4
	inc r4
	sub r1, r4
	ret
