/** @file   arm.h
 *
 *  @brief  Assembly wrappers for arm instructions.
 *
 *  @date   Last modified 11 Oct 2022
 *
 *  @author CMU 14-642
 */
#ifndef _ARM_H_
#define _ARM_H_

#include <unistd.h>

#define BUSY_LOOP(COND) while (COND) __asm("")

void init_align_prio();
void enable_prefetch_i_cache();
void enable_fpu();
void enable_interrupts();
void disable_interrupts();
void breakpoint();
void wait_for_interrupt();

#endif /* _ARM_H_ */

