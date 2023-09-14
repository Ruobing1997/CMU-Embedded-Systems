/** @file   visualizer.h
 *
 *  @brief  prototypes for color visualizer
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 11 October 2022
 *  @author CMU 14-642
**/

#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

/**
 * @brief handler for adc timer
**/
void TIMER0_IRQHandler();

/**
 * @brief handler for adc
**/
void SAADC_IRQHandler();

/**
 * @brief initialize visualizer
**/
void visualizer_init();

/**
 * @brief update visualizer color info
**/
void visualizer_color_info();

#endif /* _VISUALIZER_H_ */

