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

#endif /* _TIMER_H_ */
