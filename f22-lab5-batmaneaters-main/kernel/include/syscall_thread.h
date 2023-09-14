/** @file   syscall_thread.h
 *
 *  @brief  system calls to support thread library for lab 4
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
**/

#ifndef _SYSCALL_THREAD_H_
#define _SYSCALL_THREAD_H_

#include <unistd.h>
#include <mpu.h>
#include <syscall_mutex.h>

/** @struct interrupt_stack_frame
 *  @brief stack frame upon interrupt/exception
 */
typedef struct {
    uint32_t r0;   /**< reg r0 value */
    uint32_t r1;   /**< reg r1 value */
    uint32_t r2;   /**< reg r2 value */
    uint32_t r3;   /**< reg r3 value */
    uint32_t r12;  /**< reg r12 value */
    uint32_t lr;   /**< reg lr value */
    uint32_t pc;   /**< reg pc value */
    uint32_t xPSR; /**< reg xPSR value */
} interrupt_stack_frame;

/** @struct msp_stack_frame
 *  @brief stack frame for msp
 */
typedef struct {
    uint32_t psp; /**< reg psp value */
    uint32_t r4; /**< reg r4 value */
    uint32_t r5; /**< reg r5 value */
    uint32_t r6; /**< reg r6 value */
    uint32_t r7; /**< reg r7 value */
    uint32_t r8; /**< reg r8 value */
    uint32_t r9; /**< reg r9 value */
    uint32_t r10; /**< reg r10 value */
    uint32_t r11; /**< reg r11 value */
    uint32_t lr; /**< reg lr value */
} msp_stack_frame;

/** @struct TCB
 *  @brief define structure for TCB
 */
typedef struct {
    uint32_t C; /**< define execution time */
    uint32_t T; /**< define task period */
    uint32_t priority; /**< define priority for thread */
    uint32_t status; /**< define status **/
    msp_stack_frame* msp; /**< kernel stack **/
    void* psp_base; /**< psp base **/
    uint32_t thread_id; /**< define thread id **/
    int svc_status; /**< SVC status **/
    uint32_t computation_time; /**< computation time  **/
    uint32_t sleep_time; /**< sleep time period  **/
    uint32_t thread_time; /**< thread time  **/
    kmutex_t* waiting_mutex; /**< waiting mutex  **/
} TCB;

/** @struct kernel_struct
 *  @brief define structure for kernel_struct
 */
typedef struct {
    TCB *tcbs; /**< tcb pointer */
    uint32_t time; /**< define global time*/
    uint32_t max_threads_num; /**< define max thread number*/
    uint32_t thread_count; /**< define thread count*/
    uint32_t thread_size; /**< define thread size*/
    uint32_t current_thread; /**< define current thread*/
    uint32_t current_priority; /**< define current thread priority*/
    uint32_t protection_flag; /**< define protection flag*/
    float utilization; /**< define utilization*/
    kmutex_t* mutexes; /**< mutex array*/
    uint32_t mutex_max; /**< define max number of mutex*/
    uint32_t mutex_count; /**< define mutex count*/
    kmutex_t* current_PCP; /**< define current pcp*/
} kernel_struct;

/** @brief initialize thread switching
 *  @note  must be called before creating threads or scheduling
 *  @param max_threads      max number of threads to be created
 *  @param stack_size       stack size in words of all created stacks
 *  @param idle_fn          function pointer for idle thread, NULL for default
 *  @param mem_protect      mpu mode, either PER_THREAD or KERNEL_ONLY
 *  @param max_mutexes      max number of mutexes supported
 *  @return     0 for success, -1 for failure
 */
int sys_thread_init(uint32_t max_threads, uint32_t stack_sizes, void* idle_fn, mpu_mode mem_protect, uint32_t max_mutexes);

/** @brief create a new thread as specified, if UB allows
 *  @param fn       thread function pointer
 *  @param prio     thread priority, with 0 being highest
 *  @param C        execution time (scheduler ticks)
 *  @param T        task period (scheduler ticks)
 *  @param vargp    thread function argument
 *  @return     0 for success, -1 for failure
 */
int sys_thread_create(void* fn, uint32_t prio, uint32_t C, uint32_t T, void* vargp);

/** @brief tell the kernel to start running threads using Systick
 *  @note  returns only after all threads complete or are killed
 *  @param frequency  frequncy of context switches in Hz
 *  @return     0 for success, -1 for failure
 */
int sys_scheduler_start(uint32_t frequency);

/** @brief get the current time in ticks */
uint32_t sys_get_time();

/** @brief get the dynamic priority of the running thread */
uint32_t sys_get_priority();

/** @brief get the total elapsed time for the running thread */
uint32_t sys_thread_time();

/** @brief deschedule thread and wait until next turn */
void sys_wait_until_next_period();

/** @brief kill the running thread
 *
 *  @note  locks if main or idle thread is running or thread holds a mutex
 *
 *  @return does not return
 */
void sys_thread_kill();

/** @brief find next thread to be scheduled */
uint32_t schedule_next();

/** @brief round robin scheduler */
uint32_t round_robin_scheduler();

/** @brief function for update status everytime systick is called */
void Systick_RMS_scheduler();

/** @brief RMS scheduler to find next thread to be scheduled */
uint32_t RMS_Scheduler_Highest();

/** @brief RMS scheduler with PCP to find next thread to be scheduled */
uint32_t RMS_PCP_Scheduler_Highest();

/** @brief check mutex block condition */
void check_block(kmutex_t* mutex);
/** @brief check mutex unblock condition */
void check_unblock(kmutex_t* mutex);
/** @brief calculate utilization for UB test */
float calculate_utilization(uint32_t C, uint32_t T);
/** @brief check if mutex exists */
uint32_t check_handle(uint32_t mutex_index);
/** @brief function for locking mutex */
void mutex_check_block(kmutex_t* mutex);
/** @brief function for unlocking mutex*/
void mutex_check_unblock(kmutex_t* mutex);
/** @brief RMS scheduler with PCP*/
uint32_t RMS_PCP_Scheduler();
/** @brief function to check mutex block for pcp*/
void pcp_check_block(kmutex_t* mutex);
/** @brief function to check mutex unblock for pcp*/
void pcp_check_unblock(kmutex_t* mutex);
/** @brief function for check current PCP*/
uint32_t check_curr_PCP_NULL(TCB* tcb, uint32_t index);
/** @brief function for check prio ceil*/
uint32_t check_Prio_Ceil(TCB* tcb, uint32_t index);
/** @brief check valid condition*/
uint32_t check_valid(uint32_t curr);

#endif /* _SYSCALL_THREAD_H_ */
