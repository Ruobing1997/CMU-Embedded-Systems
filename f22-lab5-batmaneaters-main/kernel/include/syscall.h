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

/**
 * @brief syscall for malloc
**/
void* sys_sbrk(int incr);

/**
 * @brief syscall for setting pixel color
**/
void sys_pix_set(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief syscall for writing to serial
**/
int sys_write(int file, char* ptr, int len);

/**
 * @brief syscal for reading from serial
**/
int sys_read(int file, char* ptr, int len);

/**
 * @brief syscall for exit function
**/
void sys_exit(int status);

/**
 * @brief syscall for listening button press
**/
uint8_t sys_button_press();

/**
 * @brief syscall for initializing randomly
**/
uint8_t sys_random_initial();

/**
 * @brief syscall for starting visualizer
**/
void sys_visual_start();

/**
 * @brief syscall for stopping visualizer
**/
void sys_visual_stop();

/**
 * @brief syscall for starting flash led
**/
void sys_start_flash(int freq);

/**
 * @brief syscall for stopping flash led
**/
void sys_stop_flash();


#endif /* _SYSCALL_H_ */
