.global maintest
.text

main: 
	mov #4, r2
	dec r2
	decd r2
	inc r2
	incd r2
	ret
