/**
 * @file   svc_stubs.S
 *
 * @brief  Stub functions for 14642 syscalls
 *
 * @date   last updated October 6, 2022
 *
 * @author Patrick Tague <tague@cmu.edu>
 */

.cpu cortex-m4
.syntax unified
.section .svc_stub
.thumb

#include "../../kernel/include/svc_num.h"

.global _start
_start:
  bkpt

.global _sbrk
_sbrk:
  SVC #0x0
  BX  lr

.global _write
_write:
  SVC #0x1
  BX  lr

.global _close
_close:
  bkpt

.global _fstat
_fstat:
  @ bkpt
  SVC #0x3
  BX  lr


.global _isatty
_isatty:
  @ bkpt
  SVC #0x4
  BX  lr

.global _lseek
_lseek:
  @ bkpt
  SVC #0x5
  BX  lr

.global _read
_read:
  SVC #0x6
  BX  lr
  

.global _gettimeofday
_gettimeofday:
  bkpt

.global _times
_times:
  bkpt

.global _getpid
_getpid:
  bkpt

.global _kill
_kill:
  bkpt

.global _exit
_exit:
  @ call exit syscall function
  @ ? why we need add and sub here?
  @ how to pass parameters here?
  @ how to process return value
  SVC #0x7
  BX  lr

  @ bkpt

.global delay_ms
delay_ms:
  SVC #0x16
  BX  lr
  @ bkpt
  
.global lux_read
lux_read:
  SVC #0x17
  BX  lr
  @ bkpt

.global pix_set
pix_set:
  SVC #0x18
  BX  lr
  @ bkpt

