/**
 * @file    syscall.h
 *
 * @brief   declarations of system calls for user-space access to I/O, peripherals, etc.
 *
 * @date    October 17, 2020
 *
 * @author  Patrick Tague <tague@cmu.edu>
 */

#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

void* sys_sbrk(int incr);

int sys_read(int file, char* ptr, int len);

int sys_write(int file, char* ptr, int len);

void sys_exit(int status);

void sys_delay_ms(uint32_t ms);

uint16_t sys_lux_read();

void sys_pix_set(uint8_t red, uint8_t green, uint8_t blue);

#endif /* _SYSCALLS_H_ */
