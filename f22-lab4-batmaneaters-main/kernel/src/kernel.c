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
#include <syscall_thread.h>

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
    enable_fpu();
    enable_reset();

    mm_protect_start();

    enter_user_mode();

    BUSY_LOOP(1);
    return 0;
}
