/**print
 * @file: printk.h -- string printing capability that wraps RTT
 *                 -- adapted from SEGGER implementation
 *                 -- not for public release, do not share
 * @date: last modified 23 February 2021
**/

#ifndef _PRINTK_H_
#define _PRINTK_H_

#include <rtt.h>

#define RTT_PRINTK_BUFFER_SIZE	64

#define FORMAT_LEFT_JUSTIFY   (1u << 0)
#define FORMAT_PAD_ZERO       (1u << 1)
#define FORMAT_PRINT_SIGN     (1u << 2)
#define FORMAT_ALTERNATE      (1u << 3)
/**
 * @brief Definition for the data structure used for rtt print
 * @param p_buffer: start of buffer
 * @param buffer_size: buffer size (bytes)
 * @param count: count for the printed string
 * @param return_value: return value for printk function
 * @param buffer_index: used buffer index for printk
**/
typedef struct {
  char* p_buffer;
  uint32_t buffer_size;
  uint32_t count;
  int return_value;
  uint32_t buffer_index;
} rtt_printk_desc_t;

int rtt_printk(uint32_t buffer_index, const char * s_fmt, ...);
int rtt_vprintk(uint32_t buffer_index, const char * s_fmt, va_list *p_params);
int printk(const char * s_fmt, ...);

#endif // #ifndef _PRINTK_H_
