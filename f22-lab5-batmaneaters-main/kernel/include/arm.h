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

#define intrinsic __attribute__((always_inline)) static inline
#define BUSY_LOOP(COND) while (COND) __asm("")

void init_align_prio();
void enable_prefetch_i_cache();
void enable_fpu();
void pend_pendsv();
void clear_pendsv();
int get_svc_status();
void set_svc_status(int status);

/** @brief set breakpoint to current instruction */
intrinsic void breakpoint(){
    __asm volatile("bkpt");
}

/** @brief wait for interrupt to occur */
intrinsic void wait_for_interrupt(){
    __asm volatile("wfi");
}

/** @brief enable interrupts */
intrinsic void enable_interrupts(){
    __asm volatile("cpsie i");
}

/** @brief disable interrupts */
intrinsic void disable_interrupts(){
    __asm volatile("cpsid i");
}

/** @brief data synchronization memory barrier */
intrinsic void data_sync_barrier() {
    __asm volatile("dsb");
}

/** @brief instructino syncronization barrier */
intrinsic void instruction_sync_barrier() {
    __asm volatile("isb");
}

/** @brief wrapper for asm strex (store exclusive) */
intrinsic uint32_t store_exclusive(uint32_t *addr, uint32_t val) {
    uint32_t result;
    __asm volatile("strex %0, %1, [%2]" : "=r" (result) : "r" (val), "r" (addr));
    return(result);
}

/** @brief wrapper for asm ldrex (load exclusive) */
intrinsic uint32_t load_exclusive(uint32_t *addr) {
    uint32_t result;
    __asm volatile("ldrex %0, [%1]" : "=r" (result) : "r" (addr));
    return(result);
}

/** @brief saves interrupt enabled state and disables interrupts */
intrinsic int save_interrupt_state_and_disable() {
    int result;
    int disable_constant = 1;
    __asm volatile("mrs %0, PRIMASK" : "=r" (result));
    __asm volatile("msr PRIMASK, %0" :: "r" (disable_constant));
    return result;
}

/** @brief restores saved interrupt enable state */
intrinsic void restore_interrupt_state(int state) {
    __asm volatile("msr PRIMASK, %0" :: "r" (state));
}

#undef intrinsic
#endif /* _ARM_H_ */

