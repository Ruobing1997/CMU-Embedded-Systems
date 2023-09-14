/** @file   svc_stubs.S
 *
 *  @brief  Stub functions for 14642 syscalls
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 14 October 2022
 *  @author CMU 14-642
**/

.cpu cortex-m4
.syntax unified
.section .svc_stub
.thumb

#include "../../kernel/include/svc_num.h"

.global _sbrk
_sbrk:
  SVC SVC_SBRK
  BX  lr

.global _write
_write:
  SVC SVC_WRITE
  BX  lr

.global _fstat
_fstat:
  @ bkpt
  SVC SVC_FSTAT
  BX  lr

.global _isatty
_isatty:
  @ bkpt
  SVC SVC_ISATTY
  BX  lr

.global _lseek
_lseek:
  @ bkpt
  SVC SVC_LSEEK
  BX  lr

.global _read
_read:
  SVC SVC_READ
  BX  lr

.global _getpid
_getpid:
  @ bkpt
  mov r0, #1
  bx lr

.global _exit
_exit:
  @ call exit syscall function
  @ ? why we need add and sub here?
  @ how to pass parameters here?
  @ how to process return value
  SVC SVC_EXIT
  BX  lr

.global thread_init
thread_init:
  @ bkpt
  SVC SVC_THR_INIT
  BX  lr

.global thread_create
thread_create:
  @ bkpt
  SVC SVC_THR_CREATE
  BX  lr

.type thread_kill, %function
.global thread_kill
thread_kill:
  @ bkpt
  SVC SVC_THR_KILL
  BX  lr

.global scheduler_start
scheduler_start:
  @ bkpt
  SVC SVC_SCHD_START
  BX  lr

.global mutex_init
mutex_init:
  @ bkpt
  SVC SVC_MUT_INIT
  BX  lr

.global mutex_lock
mutex_lock:
  @ bkpt
  SVC SVC_MUT_LOK
  BX lr

.global mutex_unlock
mutex_unlock:
  @ bkpt
  SVC SVC_MUT_ULK
  BX  lr

.global wait_until_next_period
wait_until_next_period:
  @ bkpt
  SVC SVC_WAIT
  BX  lr

.global get_time
get_time:
  @ bkpt
  SVC SVC_TIME
  BX  lr

.global get_priority
get_priority:
  @ bkpt
  SVC SVC_PRIORITY
  BX  lr

.global thread_time
thread_time:
  @ bkpt
  SVC SVC_THR_TIME
  BX  lr

.global delay_ms
delay_ms:
  bkpt
  
.global lux_read
lux_read:
  bkpt

.global pix_set
pix_set:
  bkpt

@ define default_idle function
.thumb_func
.global default_idle
default_idle:
  wfi
  b default_idle

/* the following stubs do not need to be implemented */

.global _start
_start:
  bkpt

.global _close
_close:
  bkpt
  
.global _kill
_kill:
  bkpt
  
.global _gettimeofday
_gettimeofday:
  bkpt

.global _times
_times:
  bkpt


