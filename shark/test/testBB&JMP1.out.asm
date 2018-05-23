.global maintest
.text 
xx_test_testBB&JMP1xoutxbc_1:
    mov.w #1, r1
    mov.w #2, r2
    jmp xx_test_testBB&JMP1xoutxbc_2
xx_test_testBB&JMP1xoutxbc_2:
    sub.w r2, r1
    jmp xx_test_testBB&JMP1xoutxbc_3
xx_test_testBB&JMP1xoutxbc_3:
    add.w r1, r2
    ret.w 
