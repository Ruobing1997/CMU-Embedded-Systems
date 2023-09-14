/** @file   reset.c
 *
 *  @brief  implementation of pin reset
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
**/
#include <reset.h>
#include <stdint.h>
#include <printk.h>
#include <gpio.h>

/**
 * @brief enable user button
 * @return None
**/
void enable_userSw() {
    // switch button, p1.02
    // enable GPIO pin for userSw button
    gpio_init(1, 2, 0, 0, 0);

    // GPIOTE config base address
    uint32_t *config_address = (uint32_t *)0x40006514;

    // configure mode as input
    *config_address = 1;
    // pin number is 2
    *config_address |= (0x2 << 8);
    // configure port, port number is 1
    *config_address |= (1 << 13);

    // falling edge
    *config_address |= (2 << 16);
    return;
}

/**
 * @brief indicate whether button is pressed
 * @return return value indicate if user button is pressed
**/
uint8_t UserSw_event() {
    uint8_t result = 0;
    // issue task and listen to event
    uint32_t *GPIOTE_event = (uint32_t *)0x40006104;
    while (1) {
        uint8_t bit_value = *GPIOTE_event & 1;
        if (bit_value) {
            // printk("user button pressed\n");
            result = 1;
            break;
        }
    }

    // clear current event for future use
    *GPIOTE_event = 0;

    return result;
}



