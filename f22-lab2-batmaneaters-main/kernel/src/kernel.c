/** @file   kernel.c
 *
 *  @brief  main function for Lab 2 tasks
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 20 September 2022
 *  @author xingzhi2, ruobing2
**/

#include <arm.h>
#include <gpio.h>
#include <printk.h>
#include <adc.h>
#include <i2c.h>
#include <pix.h>
#include <visualizer.h>

/**
 * @brief Function used for integration
 * @return return type int.
**/

int kernel_main() {
    breakpoint();

    // first we initialize all the component that we need in later part
    rtt_init();
    adc_init();
    pix_init();
    i2c_leader_init();
    
    // This block is used to get data from rtt_read
    uint32_t buffer_index = 0;
    // buffer used to get rrt read data
    char p_buffer[64];
    // has_data is the length of the data we get from rtt read
    uint32_t has_data = 0;
    
    // user command initialization
    char command = '0';

    // first i2c write, perform first i2c write operation
    uint8_t tx_buf[3] = {0x0, 0x0, 0x0};
    // follower address for i2c operation
    uint8_t write_follower_addr = 0x10;
    i2c_leader_write(tx_buf, 3, write_follower_addr);
    i2c_leader_stop();

    visualizer_new_collect();
    while(1) {
        // find if user type command in the console
        has_data = rtt_has_data(buffer_index);
        if (has_data != 0) {
            // if read buffer contains data, we call rtt read function to get input command
            rtt_read(buffer_index, p_buffer, has_data);
            // get user type command
            command = p_buffer[0];
        }
        // if command not empty
        if (command != '0') {
            // if command is 's', means start the party
            if (command == 's') {
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
                printk("%hu\n", get_result);
                i2c_leader_stop();

                // we set the threshold value to 50 to distinguish lightness 
                if (get_result > 50) {
                    pix_color_set(0, 0, 0);
                } else {
                    // perform adc to pwm operation here
                    while(!visualizer_data_full()) {
                        int16_t result = adc_read_pin();
                        visualizer_push_back(result);
                    }
                    uint8_t r; 
                    uint8_t g; 
                    uint8_t b; 
                    visualizer_color_info(&r, &g, &b);
                    printk("%d %d %d \n", r, g, b);
                    // set pwm value according to the converted r,g,b value
                    pix_color_set(r, g, b);
                    visualizer_new_collect();
                    for (int i = 0; i < 20000; ++i);
                }
            } else if (command == 'e') {
                // if user type into end command, we set the pwm to 0
                for (int i = 0; i < 10000; ++i);
                pix_color_set(0, 0, 0);
            } else {
                // if type into wrong command, we output wrong command
                printk("wrong command");
                printk("\n");
                for (int i = 0; i < 9999999; ++i);
            }
        }
    }
    

    BUSY_LOOP(1);
    return 0;
}
