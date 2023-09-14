/** @file   pix.h
 *
 *  @brief  constants and prototypes for ws2812b "Neopixel" PWM driver
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 05 September 2022
 *  @author CMU 14-642
 *
 *  @note   UICR and NVMC are used to drive 3.3V GPIO output voltage (default is 1.8V)
**/

#ifndef _PIX_H_
#define _PIX_H_

#include <unistd.h>
#include <stdint.h>

#define UICR_REG0   (*((uint32_t*) 0x10001304))
#define NVMC_CONFIG (*((uint32_t*) 0x4001E504))
#define NVMC_READY  (*((uint32_t*) 0x4001E400))

void pix_init();
void pix_color_set(uint8_t r, uint8_t g, uint8_t b);

#endif // _PIX_H_
