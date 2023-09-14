/** @file   adc.h
 *
 *  @brief  Prototypes for ADC initialization and read
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 05 September 2022
 *  @author CMU 14-642
**/
#ifndef _ADC_H_
#define _ADC_H_

#include <unistd.h>

/**
 * @brief Function used for adc initializatio
 * @return return None for adc_init
**/
void adc_init();
/**
 * @brief Definition for the data structure used for rtt print
 * @return return adc captured value for further usage
**/
int16_t adc_read_pin();

#endif /* _ADC_H_ */
