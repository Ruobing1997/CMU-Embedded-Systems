/**print
 * @file: printk.h -- string printing capability that wraps RTT
 *                 -- adapted from SEGGER implementation
 *                 -- not for public release, do not share
 * @date: last modified 23 February 2021
**/

#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <rtt.h>

#define RTT_PRINTK_BUFFER_SIZE	64

#define FORMAT_LEFT_JUSTIFY   (1u << 0)
#define FORMAT_PAD_ZERO       (1u << 1)
#define FORMAT_PRINT_SIGN     (1u << 2)
#define FORMAT_ALTERNATE      (1u << 3)

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

#endif // #ifndef __PRINTK_H__
