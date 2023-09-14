@ Tips
@ 1. This code is difficult to understand in its entirety, so focus on making
@    small improvements until you know what's going on.
@ 2. You have 14 GP registers that you can use instead of the stack.
@ 3. Once you have reduced the size of the code to a manageable level, you
@    will have to understand it and restructure the code.
@ 4. The programmers manual is you friend, if you are unsure about what an
@    insturction does, consult the manual!
@ 5. HINT: The source code had a lot of things declared volatile, things that
@    did not have to be volatile. Consider removing unnecessary loads.
@ 6. If you are attempting the bonus, talk to a TA/post privately on Piazza
@    before you do.

	.syntax unified
	.cpu cortex-m4
	.fpu softvfp
	.thumb

	.data
	.align	1
	.type	three, %object
	.size	three, 2
three:
	.short	3
	.align	1
	.type	thirty_three, %object
	.size	thirty_three, 2
thirty_three:
	.short	33
	.align	1
	.type	eleven, %object
	.size	eleven, 2
eleven:
	.short	11
	.align	1
	.type	five, %object
	.size	five, 2
five:
	.short	5
	.align	1
	.type	one, %object
	.size	one, 2
one:
	.short	1
	.align	2
	.type	deadbeef, %object
	.size	deadbeef, 4
deadbeef:
	.word	-559038737
	.text

	.global	optimize_me
	.thumb_func
optimize_me:
	push	{r4, r7, lr}
	sub	sp, sp, #28
	add	r7, sp, #0
	movs	r4, #0
	mov 	r6, #33
	mov 	r12, #165
	ldr 	r11, =#0xdeadbeef
	ldr 	r9, =#0x20001432

.L7:
	@ adds 	r3, r0, r4, lsl #1

	@ ldr		r5, [r0]
	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

	ldrh    r1, [r0]
	@ bkpt #0
    ldrh    r8, [r0, #2]
	smulbb 	r2, r6, r1
	smulbb 	r8, r12, r8
	subs	r10, r2, r8
	add		r10, r11
	str		r10, [r0], #4

    cmp r0, r9
    blt .L7
	@ bkpt #1
	adds	r7, r7, #28
	mov	sp, r7
	pop	{r4, r7, pc}