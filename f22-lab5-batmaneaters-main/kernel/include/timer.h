/** @file   timer.h
 *
 *  @brief  function prototypes for systick timer
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 14 October 2022
 *  @author CMU 14-642
**/

#ifndef _TIMER_H_
#define _TIMER_H_
/**
 * @brief start systick interrupt
**/
void systick_start(int frequency);
/**
 * @brief stop systick interrupt
**/
void systick_stop();

/**
 * @brief init adc timer
**/
void adc_timer_init(int freq);
/**
 * @brief start adc timer
**/
void start_adc_timer();

/**
 * @brief stop adc timer
**/
void stop_adc_timer();

/**
 * @brief init timer for flash led
**/
void init_flash_timer();

/**
 * @brief start timer for flash led
**/
void start_flash_timer();

/**
 * @brief stop timer for flash led
**/
void stop_flash_timer();

#endif /* _TIMER_H_ */
