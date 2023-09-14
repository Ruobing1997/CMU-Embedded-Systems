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

