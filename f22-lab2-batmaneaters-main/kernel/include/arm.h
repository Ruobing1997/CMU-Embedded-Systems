/** @file arm.h
 *
 *  @brief  Assembly wrappers for arm instructions.
 *
 *  @date   Last modified 05 Sept 2022
 *
 *  @author CMU 14-642
 */
#ifndef _ARM_H_
#define _ARM_H_

#include <unistd.h>

#define BUSY_LOOP(COND) while (COND) __asm("")

void enable_prefetch_i_cache();
void enable_fpu();
void enable_interrupts();
void disable_interrupts();
void breakpoint();
void wait_for_interrupt();
uint32_t load_exclusive_register(uint32_t* addr);
uint32_t store_exclusive_register(uint32_t* addr, uint32_t val);

#endif /* _ARM_H_ */
