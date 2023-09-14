/**
 * @file: rtt.h -- prototypes and defines for RTT (real-time transfer)
 *              -- adapted from SEGGER implementation
 *              -- not for public release, do not share
 * @date: last modified 23 February 2021
**/

#ifndef _RTT_H_
#define _RTT_H_

#include <unistd.h>
#include <stdarg.h>

#define RTT_MAX_UP_BUFFERS		1
#define RTT_MAX_DOWN_BUFFERS	1
#define BUFFER_SIZE_UP			1024
#define BUFFER_SIZE_DOWN		16

#define MIN(a, b)  			(((a) < (b)) ? (a) : (b))
#define MAX(a, b)  			(((a) > (b)) ? (a) : (b))
#ifndef NULL   // just in case
  #define NULL 				0
#endif

#define RTT__DMB() __asm volatile ("dmb\n" : : :);

/**
 * definition of a circular "ring" buffer for the device -> host "up" buffer
 * @brief This data structure defines the rtt_buffer_up_t used by rtt_write function
 * @param name: name, e.g., "Terminal"
 * @param p_buffer: start of buffer
 * @param buffer_size: buffer size (bytes), not including spacer between end/start
 * @param pos_wr: position of next write
 * @param pos_rd: position of next read by host (volatile because host modifies)
 * @param flags: config flags
**/
typedef struct {
  const char* name;          // name, e.g., "Terminal"
  char* p_buffer;            // start of buffer
  uint32_t buffer_size;      // buffer size (bytes), not including spacer between end/start
  uint32_t pos_wr;           // position of next write
  volatile uint32_t pos_rd;  // position of next read by host (volatile because host modifies)
  uint32_t flags;            // config flags
} rtt_buffer_up_t;

/**
 * definition of a circular "ring" buffer for the host -> device "down" buffer
 * @brief This data structure defines the rtt_buffer_up_t used by rtt_write function
 * @param name: name, e.g., "Terminal"
 * @param p_buffer: start of buffer
 * @param buffer_size: buffer size (bytes), not including spacer between end/start
 * @param pos_wr: position of next write
 * @param pos_rd: position of next read by host (volatile because host modifies)
 * @param flags: config flags
**/
typedef struct {
  const char* name;          // name, e.g., "Terminal"
  char* p_buffer;            // start of buffer
  uint32_t buffer_size;      // buffer size (bytes), not including spacer between end/start
  volatile uint32_t pos_wr;  // position of next write by host (volatile because host modifies)
  uint32_t pos_rd;           // position of next read by target
  uint32_t flags;            // config flags
} rtt_buffer_down_t;

/**
 * RTT control block with config details
**/
typedef struct {
  char id[16];                              		// string for debug ID
  uint32_t num_up_buffers;           			    // initialized to RTT_MAX_UP_BUFFERS
  uint32_t num_down_buffers;         			    // initialized to RTT_MAX_DOWN_BUFFERS
  rtt_buffer_up_t up_buffers[RTT_MAX_UP_BUFFERS];	// buffers to send info "up" : device -> host
  rtt_buffer_down_t down_buffers[RTT_MAX_DOWN_BUFFERS]; // buffers to send info "down" : host -> device
} rtt_control_t;

void rtt_init();
uint32_t rtt_write(uint32_t buffer_index, const void* p_buffer, uint32_t num_bytes);
uint32_t rtt_read(uint32_t buffer_index, void* p_buffer, uint32_t buffer_size);
uint32_t rtt_has_data(uint32_t buffer_index);

#endif // #ifndef _RTT_H_
