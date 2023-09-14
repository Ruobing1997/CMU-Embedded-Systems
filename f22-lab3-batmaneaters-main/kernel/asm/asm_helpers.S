.cpu cortex-m4
.syntax unified
.thumb

.section .text

.thumb_func
.global enter_user_mode
enter_user_mode:
    @ bkpt
    @ load the process stack pointer
    @ load psp to register r0
    LDR r0, =__psp_stack_base
    MSR psp, r0
    
    @ set SPSEL bit to 1, let PSP as the current stack pointer
    @ set nPRIV to 1, define the thread mode as unprivileged
    MOV r1, #0x3
    MSR control, r1

    isb
    bl launch_main
    @ cannot allow this to return or the wrong stack will be used...

