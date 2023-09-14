#include <arm.h>
#include <syscall.h>
#include <unistd.h>
#include <stdint.h>
#include <rtt.h>
#include <printk.h>
#include <pix.h>
#include <i2c.h>
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
    (void) incr;
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
    printk("%d", status);
    printk("\n");

    gpio_init(1, 15, MODE_OUTPUT, OUTPUT_S0H1, PUPD_PULL_UP);
    if (status != 0) {
        gpio_set(1, 15);
    }
    disable_interrupts();
    wait_for_interrupt();
}


/* syscalls for custom user projects */

/**
 * @brief implement delay syscall function
 * (1) call systick_delay function
 * @param ms time for delay function
 * @return None
**/
void sys_delay_ms(uint32_t ms) {
    (void) ms;
    systick_delay(ms);
}

/**
 * @brief implement read lux sensor data
 * (1) first write 0x4 command to i2c to request sensor data
 * (2) after the first write function, call i2c_leader_read to read lux sensor data
 * @return uint16_t result for sensor data
**/
uint16_t sys_lux_read() {
    // follower address for i2c operation
    uint8_t write_follower_addr = 0x10;
    // rtt write send request command code 0x4 to request lux sensor data
    uint8_t tx_buf1[1] = {0x4};
    i2c_leader_write(tx_buf1, 1, write_follower_addr);

    // perform delay between i2c write and i2c read
    for (int i = 0; i < 10000; ++i);

    // get output result from i2c read function
    uint16_t output;
    uint8_t rx_len = 2;
    uint8_t read_follower_addr = 0x10;

    uint16_t get_result = i2c_leader_read((uint8_t*)&output, rx_len, read_follower_addr);
    i2c_leader_stop();

    return get_result;
}

/**
 * @brief implement pixel set function
 * (1) call pix_color_set function to set neopixel value
 * @param red red value for pixel set function
 * @param green green value for pixel set function 
 * @param blue blue value for pixel set function
 * @return None
**/
void sys_pix_set(uint8_t red, uint8_t green, uint8_t blue) {
    (void) red;
    (void) green;
    (void) blue;
    pix_color_set(red, green, blue);
}

