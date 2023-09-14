/** @file   arm.c
 *
 *  @brief  Assembly wrappers for arm instructions.
 *
 *  @date   Last modified 11 Oct 2022
 *
 *  @author CMU 14-642
**/

#include <arm.h>
#include <unistd.h>

/* @brief Configuration and Control Register */
#define CCR ((volatile uint32_t *) 0xE000ED14)
/* @brief System Handler Priority Register #2 */
#define SHPR2 ((volatile uint32_t *) 0xE000ED1C)
/* @brief I-code cache configuration register, to configure instr caching on nrf52840 */
#define ICACHECNF ((volatile uint32_t *) 0x540)
/* @brief Register to enable/disable fpu */
#define CPACR ((volatile uint32_t *) 0xE000ED88)
/* @brief FPU control data */
#define FPCCR ((volatile uint32_t *) 0xE000EF34)

/**
 * @brief  disable stack alignment and set SVC handler priority
 */
void init_align_prio() {
  // stack alignment
  *CCR &= ~(0x1 << 9);

  // Lower SVC handler priority
  *SHPR2 |= 0x20000000;

  __asm volatile("dsb");
  __asm volatile("isb");
}

void enable_prefetch_i_cache(){
    // enable bit
    *ICACHECNF |= (0x1 << 0);
}

void enable_fpu(){
  *CPACR |= (0xF << 20);
  *FPCCR |= (0x1 << 31);
  __asm volatile("dsb");
  __asm volatile("isb");
}

void enable_interrupts(void){
  __asm volatile("CPSIE f");
}

void disable_interrupts(void){
  __asm volatile("CPSID f");
}

void breakpoint(void){
  __asm volatile("bkpt");
}

void wait_for_interrupt(void){
  __asm volatile("wfi");
}

