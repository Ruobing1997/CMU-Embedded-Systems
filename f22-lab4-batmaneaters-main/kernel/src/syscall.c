/** @file   syscall.c
 *
 *  @brief  base syscall implementations for Lab 4 tasks
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
**/

#include <arm.h>
#include <syscall.h>
#include <unistd.h>
#include <stdint.h>
#include <rtt.h>
#include <printk.h>
#include <timer.h>
#include <gpio.h>

/**
 * @brief global extern variable, define the heap_base
**/
extern char __heap_base;
/**
 * @brief global extern variable, define the heap_limit
**/
extern char __heap_limit;


/**
 * @brief current_brk is used for keeping track of where the current program is
**/
char* current_brk = NULL;

/**
 * @brief implement malloc internal syscall
 * (1) if current_brk is NULL, assign the heap_base to current_brk
 * (2) find out if current_brk + incr is large than heap_limit or not
 * (3) if larger, return (void*)-1
 * (4) if not, return the starting location
 * @param incr increment space for malloc syscall
 * @return void pointer, starting location for allocated space
**/
void* sys_sbrk(int incr) {
    // (void) incr;
    if (current_brk == NULL) {
        current_brk = &__heap_base;
    }
    
    char *temp = current_brk + incr;
    // find out if run out of memory
    if (temp > &__heap_limit) {
        return (void *)-1;
    } else{
        // move heap_size
        current_brk = temp;
        // return the starting location where it was moved from
        return (void*)(current_brk - incr);
    }
}

/**
 * @brief implement read syscall function
 * (1) find out the file descriptor is 0, 0 is stdin
 * (2) first cast arguments to the required type for rtt_read
 * (3) call rtt_read for sys_read
 * @param file file descriptor for sys_read, we only support stdin
 * @param ptr buffer for putting result in
 * @param len length for sys_read
 * @return length of read bytes
**/
int sys_read(int file, char* ptr, int len) {
    (void) file;
    (void) ptr;
    (void) len;
    if (file == 0) {
        void* buffer = (void *)ptr;
        uint32_t num_bytes = (uint32_t)len;
        
        int buffer_read = (int)rtt_read(0, buffer, num_bytes);

        return buffer_read;
    } else {
        return -1;
    }
    
}

/**
 * @brief implement write syscall function
 * (1) find out if file descriptor is 1, 1 is stdout
 * (2) first cast arguments to the required type
 * (3) call rtt_write for sys_write function
 * @param file file descriptor for sys_read, we only support stdin
 * @param ptr buffer used for writing
 * @param len length for sys_write
 * @return length of bytes to be written
**/
int sys_write(int file, char* ptr, int len) {
    (void) file;
    (void) ptr;
    (void) len;
    if (file == 1) {
        void* buffer = (void *)ptr;
        uint32_t num_bytes = (uint32_t)len;
        // breakpoint();
        int buffer_write = (int)rtt_write(0, buffer, num_bytes);

        return buffer_write;
    } else {
        return -1;
    }
    
}

/**
 * @brief implement write syscall function
 * (1) first print the status code
 * (2) and then turn on the red status LED light
 * @param status status code for exit function
 * @return None
**/
void sys_exit(int status) {
    (void) status;
    // print out the exit code
    disable_interrupts();
    printk("%d", status);
    printk("\n");

    gpio_init(1, 15, MODE_OUTPUT, OUTPUT_S0H1, PUPD_PULL_UP);
    if (status != 0) {
        gpio_set(1, 15);
    }
    while(1) {
        wait_for_interrupt();
    }
}

