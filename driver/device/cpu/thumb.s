.global __gnu_thumb1_case_si

.thumb

.align 2

.thumb_func

__gnu_thumb1_case_si:
	push {r0, r1}
    mov r1, lr
    add r1, r1, #2
    lsr r1, r1, #2
    lsl r0, r0, #2
    lsl r1, r1, #2
    ldr r0, [r1, r0]
    add r0, r0, r1
    mov lr, r0
    pop {r0, r1}
    mov pc, lr
