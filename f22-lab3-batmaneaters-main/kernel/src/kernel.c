#include <arm.h>
#include <timer.h>
#include <rtt.h>
#include <reset.h>
#include <printk.h>
#include <visualizer.h>
#include <stdint.h>
#include <pix.h>
#include <adc.h>
#include <i2c.h>

extern void enter_user_mode();

int kernel_main() {
    init_align_prio();      // <-- do not remove this instruction
    breakpoint();
    // test systick
    // ? how to call the systick_handler function
    // ? what should we do in the systick_start function
    // ? where and how to set the RVR register value
    // start the timer
    rtt_init();
    adc_init();
    pix_init();
    i2c_leader_init();
    uint8_t tx_buf[3] = {0x0, 0x0, 0x0};
    // follower address for i2c operation
    uint8_t write_follower_addr = 0x10;
    i2c_leader_write(tx_buf, 3, write_follower_addr);
    i2c_leader_stop();
    
    enable_reset();

    // // systick_start();
    // int freq = 1000;
    // visualizer_init(freq);

    enter_user_mode();
    
    

    BUSY_LOOP(1);

    return 0;
}

