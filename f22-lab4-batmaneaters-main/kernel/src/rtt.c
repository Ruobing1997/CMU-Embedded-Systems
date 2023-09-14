/*
 * @file: rtt.c -- real-time transfer for debugging
 *
 * @date: last updated 23 February 2021
 * 
 * @desc: this is the RTT client to run on the "device" side to interact with "host"
 *        -- RTT operates over a debug channel using a "control block" data structure
 *        -- multiple channels of input and output can be supported (up to 16) for multiplexing
 *        -- control block contains:
 *             ---- an array of "up" buffers for writing data to the host (buffer 0 ==> "terminal")
 *             ---- an array of "down" buffers for reading input from the host (buffer 0 ==> "terminal")
 *             ---- size of each up and down buffer array
 *             ---- an id that allows the RTT tool on "host" to identify the control block
 *        -- each up/down buffer is a ring buffer or circular array, meaning reads and writes wrap back to index 0
 *        -- each ring buffer maintains two indices for last written and last read entry
**/

#include <rtt.h>
#include <printk.h>
#include <stdint.h>

extern rtt_control_t __rtt_start;

static char up_buffer[BUFFER_SIZE_UP];
static char down_buffer[BUFFER_SIZE_DOWN];

/*
 * @brief: initialize control blocks, must be called before any other rtt ops
 * 
 * @note: this is provided for you and should not be changed in any way
 */
void rtt_init() {
  rtt_control_t* p;
  p = &__rtt_start;
  p->num_up_buffers = RTT_MAX_UP_BUFFERS;
  p->num_down_buffers = RTT_MAX_DOWN_BUFFERS;

  p->up_buffers[0].name = "Terminal";
  p->up_buffers[0].p_buffer = up_buffer;
  p->up_buffers[0].buffer_size = BUFFER_SIZE_UP;
  p->up_buffers[0].pos_rd = 0;
  p->up_buffers[0].pos_wr = 0;
  p->up_buffers[0].flags = 2;

  p->down_buffers[0].name = "Terminal";
  p->down_buffers[0].p_buffer = down_buffer;
  p->down_buffers[0].buffer_size = BUFFER_SIZE_DOWN;
  p->down_buffers[0].pos_rd = 0;
  p->down_buffers[0].pos_wr = 0;
  p->down_buffers[0].flags = 2;

  p->id[7] = 'R'; p->id[8] = 'T'; p->id[9] = 'T';
  RTT__DMB();
  p->id[0] = 'S'; p->id[1] = 'E'; p->id[2] = 'G'; p->id[3] = 'G'; p->id[4] = 'E'; p->id[5] = 'R';
  RTT__DMB();
  p->id[6] = ' ';
  RTT__DMB();
  p->id[10] = '\0'; p->id[11] = '\0'; p->id[12] = '\0'; p->id[13] = '\0'; p->id[14] = '\0'; p->id[15] = '\0';
  RTT__DMB();
}

/**
 * @brief Function used for rtt writing
 * @param buffer_index: index for buffer to be used
 * @param p_buffer: buffer used to store the write content
 * @param num_bytes: bytes need to be write into the buffer
 * @return uint32_t, length of bytes stored in rtt
**/
uint32_t rtt_write(uint32_t buffer_index, const void* p_buffer, uint32_t num_bytes) {
  (void)buffer_index;
  (void)p_buffer;
  (void)num_bytes;

  // pushes an array of characters into 0th up_buffer
  char* get_buffer = (char *)p_buffer;
  rtt_control_t *p;
  p = &__rtt_start;
  uint32_t position_write = p->up_buffers[buffer_index].pos_wr;
  for (uint32_t i = position_write; i <  position_write + num_bytes; i++) {
    p->up_buffers[buffer_index].p_buffer[i % BUFFER_SIZE_UP] = get_buffer[i - position_write];
    p->up_buffers[buffer_index].pos_wr += 1;
    p->up_buffers[buffer_index].pos_wr %= BUFFER_SIZE_UP;
    while(p->up_buffers[buffer_index].pos_wr == p->up_buffers[buffer_index].pos_rd - 1);
  }
  // for (int i = 0; i < 100000; ++i);
  // p->up_buffers[buffer_index].pos_wr = (position_write + num_bytes) % BUFFER_SIZE_UP;
  return num_bytes;
}

/**
 * @brief Function used for rtt_read
 * @param buffer_index: index for buffer to be used
 * @param p_buffer: buffer used to store reading result
 * @param buffer_size: size to be read from
 * @return uint32_t, length of bytes to be read
**/
uint32_t rtt_read(uint32_t buffer_index, void* p_buffer, uint32_t buffer_size) {
  (void)buffer_index;
  (void)p_buffer;
  (void)buffer_size;


  rtt_control_t *p;
  p = &__rtt_start;
  uint32_t position_read = p->down_buffers[buffer_index].pos_rd;

  uint32_t rtt_data_size = rtt_has_data(buffer_index);
  if (rtt_data_size < buffer_size) {
    buffer_size = rtt_data_size;
  }
  char* get_buffer = (char *)p_buffer;

  for (uint32_t i = position_read; i < position_read + buffer_size; ++i) {
    get_buffer[i - position_read] = p->down_buffers[buffer_index].p_buffer[i % BUFFER_SIZE_DOWN];
  }

  p->down_buffers[buffer_index].pos_rd = (position_read + buffer_size) % BUFFER_SIZE_DOWN;

  return buffer_size;
}

/**
 * @brief Function used for find if there is data 
 * (1) starting from the pos_rd 
 * @param buffer_index: index for buffer to be used
 * @return uint32_t, length of bytes in the buffer
**/
uint32_t rtt_has_data(uint32_t buffer_index) {
  (void)buffer_index;

  rtt_control_t *p;
  p = &__rtt_start;
  uint32_t num_bytes = 0;
  for (uint32_t i = p->down_buffers[buffer_index].pos_rd; i < p->down_buffers[buffer_index].pos_wr; ++i) {
    if (p->down_buffers[buffer_index].p_buffer[i % BUFFER_SIZE_DOWN] != '\0') {
      num_bytes++;
    }
    // num_bytes++;
  }
  return num_bytes;
}
