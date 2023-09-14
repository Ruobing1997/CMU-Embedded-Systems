/** @file   timer.c
 *
 *  @brief  timer implementations for systick timer
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
**/

#include <timer.h>
#include <unistd.h>
#include <stdint.h>
#include <gpio.h>


/**
 * @brief define CSR register for later use
**/
uint32_t* CSR_reg = (uint32_t *)0xE000E010;

/**
 * @brief define RVR register for later use
**/
uint32_t* RVR_reg = (uint32_t *)0xE000E014;

/**
 * @brief start systick interrupt
 * @param frequency frequency to be used for systick interrupt
 * @return None
**/
void systick_start(int frequency) {
    (void)frequency;
    // set CSR register value
    *CSR_reg = 0x7;
    // get RVR register value
    // ??? 
    *RVR_reg = (16000000 / frequency);

    // set NVIC register
    uint32_t* nvic_iser = (uint32_t *)0xE000E100;
    *nvic_iser |= (1 << 31);
    return;
}
/**
 * @brief stop systick interrupt
**/
void systick_stop() {
    // change the timer status registers to stop running
    *CSR_reg = 0;
    return;
}

/**
 * @brief implement syscall for starting flash timer
 * @param freq frequency for starting flash timer
 * @return None
**/
void adc_timer_init(int freq) {
    uint32_t timer0_address = 0x40008000;

    // configure timer mode
    uint32_t *timer0_mode = (uint32_t *)(timer0_address + 0x504);
    *timer0_mode = 0;

    // set prescaler value to 0
    uint32_t *prescalar_address = (uint32_t *)(timer0_address + 0x510);
    *prescalar_address = 0;

    // configure bitmode
    uint32_t *bitmode = (uint32_t *)(timer0_address + 0x508);
    *bitmode = 3;

    // enable interrupt
    uint32_t *intenset_address = (uint32_t *)(timer0_address + 0x304);
    *intenset_address = (0x1 << 16);

    // CC[0] count
    uint32_t cc_compare = 16000000 / freq;
    uint32_t* cc0_address = (uint32_t *)(timer0_address + 0x540);
    *cc0_address = cc_compare;

    // configure NVIC register
    uint32_t* nvic_iser = (uint32_t *)0xE000E100;
    *nvic_iser |= (1 << 8);

    return;
}

/**
 * @brief implement syscall for starting adc timer
 * @return None
**/
void start_adc_timer() {
    // start task
    uint32_t *task_address = (uint32_t *)0x40008000;
    *task_address = 1;

    return;
}

/**
 * @brief implement syscall for starting adc timer
 * @return None
**/
void stop_adc_timer() {
    uint32_t *stop_address = (uint32_t *)0x40008004;
    *stop_address = 1;

    return;
}

/**
 * @brief init timer for flash led
 * @return None
**/
void init_flash_timer() {
    // timer1 mode

    // set timer mode
    uint32_t *timer1_mode = (uint32_t*)0x40009504;
    *timer1_mode = 0;

    // set prescalar
    uint32_t *prescalar = (uint32_t*)0x40009510;
    *prescalar = 0;

    // set bitmode
    uint32_t *bitmode = (uint32_t*)0x40009508;
    *bitmode = 3;

    // set intenset
    uint32_t *intenset = (uint32_t*)0x40009304;
    *intenset = (1 << 16);
    // set nvic
    uint32_t* nvic_iser = (uint32_t *)0xE000E100;
    *nvic_iser |= (1 << 9);

}

/**
 * @brief start timer for flash led with new frequency
 * @param freq new frequency for starting timer for flash led
 * @return None
**/
void start_flash_timer(int freq) {

    uint32_t *timer1_cc = (uint32_t*)0x40009540;
    *timer1_cc = 16000000 / freq;

    uint32_t *start_address = (uint32_t*)0x40009000;
    *start_address = 1;
    return;
}

/**
 * @brief stop timer for flash led
 * @return None
**/
void stop_flash_timer() {
    uint32_t *stop_address = (uint32_t*)0x40009004;
    *stop_address = 1;

    uint32_t *clear_address = (uint32_t*)0x4000900C;
    *clear_address = 1;
}

