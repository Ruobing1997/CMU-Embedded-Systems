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

void TIMER0_IRQHandler();

void SAADC_IRQHandler();

void visualizer_init(int freq);

void visualizer_color_info();

#endif /* _VISUALIZER_H_ */
