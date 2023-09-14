/** @file   timer.h
 *
 *  @brief  function prototypes for systick and adc interrupt timers
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 11 October 2022
 *  @author CMU 14-642
**/

#ifndef _TIMER_H_
#define _TIMER_H_

#include <unistd.h>
// define base address for systick registers
#define CSR_address (0xE000E010)
#define RVR_address (0xE000E014)
#define CVR_address (0xE000E018)

void TIMER0_IRQHandler();

void SysTick_Handler();

void systick_start();

void systick_delay(uint32_t ms);

void adc_timer_start(int freq);

#endif /* _TIMER_H_ */
