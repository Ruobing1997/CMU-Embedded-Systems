#include <reset.h>
#include <gpio.h>
#include <stdint.h>
#include <printk.h>

/**
 * @brief Function used for enabling reset
 * (1) first we initialize gpio for the reset button
 * (2) configure GPIOTE for gpio port and pin
 * (3) configure GPIOTE polarity
 * (4) configure GPIOTE task and enable GPIOTE intenset
 * (5) enable nvic interrupt
 * @return None
**/
void enable_reset() {
    // enable GPIO pin for reset button
    gpio_init(0, 18, MODE_INPUT, OUTPUT_S0H1, PUPD_PULL_UP);
    // configure GPIOTE
    // GPIOTE base address
    uint32_t GPIOTE_base = 0x40006000;
    uint32_t *config_address = (uint32_t *)(GPIOTE_base + 0x510);
    // configure mode as input
    *config_address = 1;
    // configure PSEL, pin number is 18
    *config_address |= (0x12 << 8);
    // configure port
    *config_address &= ~(1 << 13);
    // configure polarity
    // falling edge
    *config_address |= (2 << 16);

    uint32_t *GPIOTE_task = (uint32_t *) GPIOTE_base;
    *GPIOTE_task = 1;

    uint32_t *GPIOTE_interrupt = (uint32_t *)(GPIOTE_base + 0x304);
    *GPIOTE_interrupt = 1;
    *GPIOTE_interrupt |= (1 << 31);

    // enable interrupt
    uint32_t *nvic_iser = (uint32_t *)0xE000E100;
    *nvic_iser |= (1 << 6);
    return;
}

/**
 * @brief Function for GPIOTE_IRQHandler
 * (1) first we configure AIRCR register 
 * (2) first write 0x5FA in order not to ignore write operation
 * (3) include an infinite loop and never return
 * @return None
**/
void GPIOTE_IRQHandler() {
    uint32_t* AIRCR_address = (uint32_t *)0xE000ED0C;
    // configure AIRCR register
    *AIRCR_address = 0x05FA0004;
    // include an infinite loop
    while(1) {
        ;
    }

    return;
}

