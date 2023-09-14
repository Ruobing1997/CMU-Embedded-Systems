/** @file   pix.h
 *
 *  @brief  prototypes and constants for Neopixel driver
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 11 October 2022
 *  @author CMU 14-642
**/
 
#ifndef _PIX_H_
#define _PIX_H_

#include <unistd.h>
#define UICR_REG0 (*((uint32_t*) 0x10001304)) //!< UICR reg 0 addr
#define NVMC_CONFIG (*((uint32_t*) 0x4001E504)) //!< NVMC config register
#define NVMC_READY (*((uint32_t*) 0x4001E400)) //!< NVMC ready event

void pix_init();
void pix_color_set(uint8_t r, uint8_t g, uint8_t b);

#endif // _PIX_H_
