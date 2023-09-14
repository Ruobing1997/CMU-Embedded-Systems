/** @file   mpu.c
 *
 *  @brief  implementation of memory protection for lab 4
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
**/

#include <arm.h>
#include <mpu.h>
#include <printk.h>
#include <syscall.h>
#include <syscall_thread.h>
#include <unistd.h>
#include <stdint.h>

/** @brief mpu control register */
#define MPU_CTRL *((uint32_t*)0xE000ED94)
/** @brief mpu region number register */
#define MPU_RNR *((uint32_t*)0xE000ED98)
/** @brief mpu region base address register */
#define MPU_RBAR *((uint32_t*)0xE000ED9C)
/** @brief mpu region attribute and size register */
#define MPU_RASR *((uint32_t*)0xE000EDA0)

/** @brief MPU CTRL register flags */
//@{
/** @brief MPU CTRL enable bg bit */
#define MPU_CTRL_ENABLE_BG  (1 << 2)
/** @brief MPU CTRL enable bit */
#define MPU_CTRL_ENABLE     (1 << 0)
//@}

/** @brief MPU RNR register flags */
//@{
#define MPU_RNR_REGION      (0xFF)
/** @brief MPU max RNR region number */
#define MPU_RNR_REGION_MAX  (7)
//@}

/** @brief MPU RASR register masks */
//@{
#define MPU_RASR_XN         (1 << 28)
/** @brief define MPU RASR size */
#define MPU_RASR_SIZE       (0x3E)
/** @brief enable MPU RASR bit */
#define MPU_RASR_ENABLE     (1 << 0)
/** @brief read-only bit */
#define MPU_RASR_USER_RO    (2 << 24)
/** @brief read-write bit */
#define MPU_RASR_USER_RW    (3 << 24)
//@}

/** @brief system handler control and state register */
#define SCB_SHCRS *((uint32_t*)0xE000ED24)
/** @brief configurable fault status register */
#define SCB_CFSR *((uint32_t*)0xE000ED28)
/** @brief mem mgmt fault address register */
#define SCB_MMFAR *((uint32_t*)0xE000ED34)

/** @brief SCB_SHCRS mem mgmt fault enable bit */
#define SHCRS_MEMFAULTENA   (1 << 16)

/** @brief stacking error */
#define SCB_CFSR_STKERR     (1 << 4)
/** @brief unstacking error */
#define SCB_CFSR_UNSTKERR   (1 << 3)
/** @brief data access error */
#define SCB_CFSR_DACCVIOL   (1 << 1)
/** @brief instruction access error */
#define SCB_CFSR_IACCVIOL   (1 << 0)
/** @brief indicates the MMFAR is valid */
#define SCB_CFSR_MMFARVALID (1 << 7)

/** @brief define extern thread kill function for later use */
extern void* thread_kill;
/** @brief define __svc_stub_start symbol*/
extern char __svc_stub_start;
/** @brief define __user_rodata_start symbol*/
extern char __user_rodata_start;
/** @brief define __user_data_start symbol*/
extern char  __user_data_start;
/** @brief define __user_bss_start symbol*/
extern char __user_bss_start;
/** @brief define __heap_base symbol*/
extern char __heap_base;
/** @brief define __psp_stack_limit symbol*/
extern char __psp_stack_limit; 
/** @brief define global_stack to access global variable*/
extern kernel_struct global_stack;

/** 
 * @brief memory protection c handler function 
 * @param psp psp register to get stack location for memory protection
 * @return None
**/
void mm_c_handler(void *psp) {
    int status = SCB_CFSR & 0xFF;

    // Attempt to print cause of fault
    printk("%s: ", __func__);
    printk("Memory Protection Fault\n");
    if(status & SCB_CFSR_STKERR) printk("Stacking Error\n");
    if(status & SCB_CFSR_UNSTKERR) printk("Unstacking Error\n");
    if(status & SCB_CFSR_DACCVIOL) printk("Data access violation\n");
    if(status & SCB_CFSR_IACCVIOL) printk("Instruction access violation\n");
    if(status & SCB_CFSR_MMFARVALID) printk("Faulting Address = %x\n", SCB_MMFAR);

    // unrecoverable stack overflow
    if(psp < global_stack.tcbs[global_stack.current_thread].psp_base) {
        printk("stack overflow --> aborting\n");
        sys_exit(-1);
    }

    // Other errors can be recovered from by killing the offending thread.
    // Standard thread killing rules apply. You should halt if the thread
    // is the main or idle thread, but otherwise up to you. Manually set
    // the pc? Call a function? Context swap? TODO
    // (void) psp;
    if (global_stack.max_threads_num == 0) {
        sys_exit(-1);
    }
    // assign pc in psp
    uint32_t* get_psp = (uint32_t*)psp;
    get_psp[6] = (uint32_t)&thread_kill;
}



/** @brief  enables an aligned memory protection region
 *
 *  @param  region_number       region number to enable
 *  @param  base_address        region's base address
 *  @param  size_log2           log[2] of the region's size
 *  @param  execute          indicator if region is NOT user-executable
 *  @param  user_write_access   indicator if region is user-writable
 *
 *  @return 0 if successful, -1 on failure
 */
int mm_region_enable(uint32_t region_number, void* base_address, uint8_t size_log2, int execute, int user_write_access) {
    if(region_number > MPU_RNR_REGION_MAX) {
        printk("Invalid region number\n");
        return -1;
    }

    if((uint32_t)base_address & ((1 << size_log2) - 1)) {
        printk("Misaligned region\n");
        return -1;
    }

    if(size_log2 < 5) {
        printk("Region too small\n");
        return -1;
    }

    MPU_RNR = region_number & MPU_RNR_REGION;
    MPU_RBAR = (uint32_t)base_address;

    uint32_t size = ((size_log2 - 1) << 1) & MPU_RASR_SIZE;
    uint32_t ap = user_write_access ? MPU_RASR_USER_RW : MPU_RASR_USER_RO;
    uint32_t xn = execute ? 0 : MPU_RASR_XN;

    MPU_RASR = size | ap | xn | MPU_RASR_ENABLE;

    return 0;
}

/** 
 * @brief function for starting memory protection
 * (1) enable specific memory protection region
 * (2) enable memory segmentation fault
**/
void mm_protect_start() {
    // initialize mpu control register
    MPU_CTRL = 0;

    // enable memory protect regions
    // user code
    mm_region_enable(0, &__svc_stub_start, 14, 1, 0);

    // user rodata, read-only
    mm_region_enable(1, &__user_rodata_start, 11, 0, 0);

    // user data, read and write
    mm_region_enable(2, &__user_data_start, 10, 0, 1);

    // user bss, read and write
    mm_region_enable(3, &__user_bss_start, 10, 0, 1);

    // user heap, read and write
    mm_region_enable(4, &__heap_base, 13, 0, 1);

    //user stack, read and write
    mm_region_enable(5, &__psp_stack_limit, 11, 0, 1);
    // enable bit and bg bit
    MPU_CTRL = MPU_CTRL_ENABLE | MPU_CTRL_ENABLE_BG;

    // enable memory management faults
    SCB_SHCRS |= SHCRS_MEMFAULTENA;
    // data_sync_barrier();
}

/**
 * @brief  Disables a memory protection region.
 *
 * @param  region_number      The region number to disable.
 */
void mm_region_disable(uint32_t region_number) {
    MPU_RNR = region_number & MPU_RNR_REGION;
    MPU_RASR &= ~MPU_RASR_ENABLE;
}

/**
 * @brief  Returns ceiling (log_2 n).
 */
uint32_t mm_log2ceil_size(uint32_t n) {
    uint32_t ret = 0;
    while(n > (1U << ret)) {
        ret++;
    }
    return ret;
}

/**
 * @brief  Disable kernel protection
 */
void mm_kernel_disable() {
    MPU_RNR = 7 & MPU_RNR_REGION;
    MPU_RASR &= 0;
}

/**
 * @brief  Enable kernel memory protection
 * @param stack_address stack address for memory protection location
 * @param size size for region to be protected
 */
void mm_kernel_enable(void* stack_address, uint32_t size) {
    uint32_t calculate_size = mm_log2ceil_size(size);
    mm_region_enable(7, stack_address, calculate_size, 0, 1);
}

/**
 * @brief  Enable protection for thread's stack
 * @param stack_address stack address for thread memory protection location
 * @param size size for thread region to be protected
 */
void mm_thread_enable(void* stack_address, uint32_t size) {
    uint32_t calculate_size = mm_log2ceil_size(size);
    mm_region_enable(6, stack_address, calculate_size, 0, 1);
}