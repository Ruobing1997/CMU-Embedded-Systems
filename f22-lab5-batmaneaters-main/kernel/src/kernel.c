/** @file   kernel.c
 *
 *  @brief  starting point for kernel-space operations
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
**/

#include <arm.h>
#include <stdint.h>
#include <gpio.h>
#include <rtt.h>
#include <reset.h>
#include <user_sw.h>
#include <syscall_thread.h>
#include <visualizer.h>
#include <pix.h>
#include <adc.h>
#include <timer.h>

/** @brief define extern function for entering user mode */
extern void enter_user_mode();

/** 
 * @brief kernel main function 
 * (1) first enable fpu to calculate float 
 * (2) mm_protect_start is for starting memory protection
**/
int kernel_main() {
    init_align_prio();          // <-- do not remove

    breakpoint();
    rtt_init();
    adc_init();
    enable_fpu();
    pix_init();
    enable_userSw();
    mm_protect_start();
    adc_timer_init(10000);
    visualizer_init();
    init_flash_timer();
    gpio_init(1, 10, 1, 0, 0);
    gpio_init(1, 15, 1, 0, 0);
    enter_user_mode();

    // visualizer_stop();

    BUSY_LOOP(1);
    return 0;
}
