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
    b _usr_mod

    
