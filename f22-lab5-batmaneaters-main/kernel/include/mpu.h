/** @file   mpu.h
 *
 *  @brief  prototypes for memory protection
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
**/

#ifndef _MPU_H_
#define _MPU_H_

#include <stdint.h>
/** @enum   mpu_mode
 *  @brief  memory protection mode is PER_THREAD or KERNEL_ONLY
 */
typedef enum { PER_THREAD = 1, KERNEL_ONLY = 0 } mpu_mode;

/**
 * @brief  Returns ceiling (log_2 n).
 */
uint32_t mm_log2ceil_size(uint32_t n);
/**
 * @brief  Disables a memory protection region.
 */
void mm_region_disable(uint32_t region_number);
/**
 * @brief  enables an aligned memory protection region
 */
int mm_region_enable(uint32_t region_number, void *base_address, uint8_t size_log2, int execute, int user_write_access);
/**
 * @brief  function for starting memory protection
 */
void mm_protect_start();
/**
 * @brief  Enable protection for thread's stack
 */
void mm_thread_enable(void* stack_address, uint32_t size);
/**
 * @brief  Enable kernel memory protection
 */
void mm_kernel_enable(void* stack_address, uint32_t size);
/**
 * @brief  Disable kernel protection
 */
void mm_kernel_disable();
#endif /* _MPU_H_ */
