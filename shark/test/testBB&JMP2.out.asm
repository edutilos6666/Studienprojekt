.global maintest
.text 
xx_test_testBB&JMP2xoutxbc_1:
    mov.w #1, r1
    mov.w #2, r2
    jmp xx_test_testBB&JMP2xoutxbc_2
xx_test_testBB&JMP2xoutxbc_2:
    sub.w r2, r1
    jmp xx_test_testBB&JMP2xoutxbc_3
xx_test_testBB&JMP2xoutxbc_3:
    add.w r1, r2
    jmp xx_test_testBB&JMP2xoutxbc_4
xx_test_testBB&JMP2xoutxbc_4:
    mov.w #3, r3
    jmp xx_test_testBB&JMP2xoutxbc_5
xx_test_testBB&JMP2xoutxbc_5:
    mov.w r1, r2
    jmp xx_test_testBB&JMP2xoutxbc_6
xx_test_testBB&JMP2xoutxbc_6:
    mov.w r2, r1
    jmp xx_test_testBB&JMP2xoutxbc_7
xx_test_testBB&JMP2xoutxbc_7:
    mov.w r1, r2
    ret.w 
