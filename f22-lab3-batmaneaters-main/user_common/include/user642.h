/**
 * @file   user642.h
 *
 * @brief  function declarations for custom system calls used in userspace applications
 *
 * @date   6 October 2022
 *
 * @author CMU 14-642
 */

#ifndef _USER642_H_
#define _USER642_H_

#include <unistd.h>
#include <stdint.h>

uint16_t lux_read();

void pix_set(uint8_t red, uint8_t green, uint8_t blue);

void delay_ms(uint32_t ms);

#endif /* _USER642_H_ */
