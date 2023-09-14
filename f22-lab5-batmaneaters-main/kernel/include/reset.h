/** @file   reset.h
 *
 *  @brief  prototypes for pin reset
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 11 October 2022
 *  @author CMU 14-642
**/

#ifndef _RESET_H_
#define _RESET_H_
/** @brief function for eabling reset button */
void enable_reset();
/** @brief handler for GPIOTE */
void GPIOTE_IRQHandler();

#endif /* _RESET_H_ */
