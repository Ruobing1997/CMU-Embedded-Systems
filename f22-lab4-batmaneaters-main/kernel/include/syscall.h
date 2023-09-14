/** @file   syscall.h
 *
 *  @brief  prototypes for base system calls for lab 4
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 14 October 2022
 *  @author CMU 14-642
**/

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

void* sys_sbrk(int incr);

int sys_write(int file, char* ptr, int len);

int sys_read(int file, char* ptr, int len);

void sys_exit(int status);

#endif /* _SYSCALL_H_ */
