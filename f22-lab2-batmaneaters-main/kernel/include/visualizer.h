/** @file   visualizer.h
 *
 *  @brief  function prototypes for audio-to-LED visualizer
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 20 September 2022
 *  @author CMU 14-642
**/
#include <unistd.h>

#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

int visualizer_push_back(int16_t adc_data);
void visualizer_new_collect();
int visualizer_data_full();
int visualizer_color_info(uint8_t *r, uint8_t *g, uint8_t *b);

#endif /* _VISUALIZER_H_ */
