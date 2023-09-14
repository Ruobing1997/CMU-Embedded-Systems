/** @file   adc.h
 *
 *  @brief  prototypes for ADC driver
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 11 October 2022
 *  @author CMU 14-642
**/

#ifndef _ADC_H_
#define _ADC_H_

#include <unistd.h>

void adc_init();
void adc_read_samples(int16_t* output);

#endif /* _ADC_H_ */
