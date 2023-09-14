/** @file arm.c
 *
 *  @brief  Assembly wrappers for arm instructions.
 *
 *  @date   Last modified 05 Sept 2022
 *
 *  @author CMU 14-642
**/

#include <arm.h>
#include <unistd.h>


/** 
 * @brief Register to enable/disable fpu
**/
#define CPACR ((volatile uint32_t *) 0xE000ED88)

/** 
 * @brief FPU control data
**/
#define FPCCR ((volatile uint32_t *) 0xE000EF34)

/** 
 * @brief Hold the address of the unpopulated floating point
 * register space in stackframe
**/
#define FPCAR ((volatile uint32_t *) 0xE000EF38)

/** 
 * @brief Default values for the floating point status
 * control data (FPCCR)
**/
#define FPDSCR ((volatile uint32_t *) 0xE000EF3C)

/** 
 * @brief Read-only information about what VFP instruction features are
 * implemented
**/
#define MVFR0 ((volatile uint32_t *) 0xE000EF40)


/** 
 * @brief Read-only information about what VFP instruction features are
 * implemented
**/
#define MVFR1 ((volatile uint32_t *) 0xE000EF44)

/** 
 * @brief The I-code cache configuration register is used to configure instruction caching,
 * the only available caching on the nrf52840
**/

#define ICACHECNF ((volatile uint32_t *) 0x540)

/** 
 * @brief This function is linked for loading execlusive register 
**/
extern uint32_t load_exclusive_register(uint32_t* addr);
/** 
 * @brief This function is used for storing execlusive register
**/
extern uint32_t store_exclusive_register(uint32_t* addr, uint32_t val);

/** 
 * @brief This function is used for enabling prefetch cache
 * @return None
**/
void enable_prefetch_i_cache(){
    // enable bit
    *ICACHECNF |= (0x1 << 0);
}

/** 
 * @brief This function is used for enabling fpu
 * @return None
**/
void enable_fpu(){
  *CPACR |= (0xF << 20);
  *FPCCR |= (0x1 << 31);
  __asm volatile("dsb");
  __asm volatile("isb");
}

/** 
 * @brief This function is used for enabling interrupts
 * @return None
**/
void enable_interrupts(void){
  __asm volatile("CPSIE f");
}

/** 
 * @brief This function is used for disabling interrupts
 * @return None
**/
void disable_interrupts(void){
  __asm volatile("CPSID f");
}

/** 
 * @brief This function is used for storing execlusive register
 * @return None
**/
void breakpoint(void){
  __asm volatile("bkpt");
}

/** 
 * @brief This function is used for waiting interrupts
 * @return None
**/
void wait_for_interrupt(void){
  __asm volatile("wfi");
}
