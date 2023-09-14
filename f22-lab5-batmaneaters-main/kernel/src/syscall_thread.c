/** @file   syscall_thread.c
 *
 *  @brief  thread syscall implementation for lab 4
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 14 October 2022
 *  @author CMU 14-642
**/

#include <arm.h>
#include <mpu.h>
#include <printk.h>
#include <syscall.h>
#include <syscall_mutex.h>
#include <syscall_thread.h>
#include <unistd.h>
#include <stdint.h>
#include <svc_num.h>
#include <timer.h>

/** @brief      Initial XPSR value, all 0s except thumb bit. */
#define XPSR_INIT 0x1000000

/** @brief Return code to return to user mode with user stack.*/
#define LR_RETURN_TO_USER_PSP 0xFFFFFFFD
/** @brief define stack space limit.*/
#define STACK_SPACE_LIMIT 32768


/** @brief define running status **/
#define RUNNING_STATUS 1

/** @brief define waiting status **/
#define WAITING_STATUS 2

/** @brief define runnable status **/
#define RUNNABLE_STATUS 3

/** @brief define uninitialized status for killed thread **/
#define UNINITIALIZE_STATUS 4

/** @brief thread user stack region limits */
extern char __thread_u_stacks_limit;
/** @brief thread kernel stack region limits */
extern char __thread_k_stacks_limit;
/** @brief thread user stack region base */
extern char __thread_u_stacks_base;
/** @brief thread kernel stack region base */
extern char __thread_k_stacks_base;
/** @brief define extern function to use thread_kill */
extern void thread_kill();
/** @brief define extern idle function to use */
extern void default_idle();

/** @brief precomputed values for UB test */
float ub_table[] = {
    0.000, 1.000, .8284, .7798, .7568,
     .7435, .7348, .7286, .7241, .7205,
     .7177, .7155, .7136, .7119, .7106,
     .7094, .7083, .7075, .7066, .7059,
     .7052, .7047, .7042, .7037, .7033,
     .7028, .7025, .7021, .7018, .7015,
     .7012, .7009
};

/** @brief define global TCB array */
TCB TCB_array[16];

/** @brief define global stack variable to use */
kernel_struct global_stack;

/** @brief define struct for mutex */
kmutex_t global_mutexes[32];
/** @brief define systick time */
uint32_t tick = 0;

/**
 * @brief  systick c handler function
 * (1) increase global time everytime systick is called
 * (2) update RMS status
 * (3) call pendsv to start context switch
 */
void systick_c_handler() {
    tick++;
    global_stack.time++;
    Systick_RMS_scheduler();
    // enable pendsv interrupt
    pend_pendsv();
}

/**
 * @brief  pendsv c handler function
 * @param msp msp register to get stack address for thread
 * (1) call scheduler function to find next thread to be scheduled
 * (2) set and get svc status
 * (3) do context switch and return corresponding thread stack address
 */
void *pendsv_c_handler(void *msp){
    (void)msp;
    // copy state into TCB, call scheduler to figure out new thread, return back to asm handler, copy msp
    // return a stack pointer as a result
    // call function from syscall
    // msp stack including the psp stack?

    // when there are no more threads
    if (global_stack.thread_count == 0) {
        systick_stop();
        uint32_t main_thread_prio = global_stack.max_threads_num + 1;
        global_stack.max_threads_num = 0;
        // return main thread context
        return (void*)global_stack.tcbs[main_thread_prio].msp;
    }
    // run scheduling algorithm
    // get array index for next scheduled thread
    uint32_t schedule_thread = schedule_next();
    // if different thread scheduled to run, do context switching
    if (global_stack.current_thread != schedule_thread) {
        if (global_stack.protection_flag == 1) {
            mm_thread_enable(global_stack.tcbs[schedule_thread].psp_base, global_stack.thread_size);
        }
        // how to save SVC status???
        int status = get_svc_status();
        global_stack.tcbs[global_stack.current_thread].svc_status = status;
        // save current thread to current thread TCB
        global_stack.tcbs[global_stack.current_thread].msp = (msp_stack_frame*)msp;
        // update current thread
        global_stack.current_thread = schedule_thread;
        // set next thread svc status
        global_stack.current_priority = global_stack.tcbs[schedule_thread].priority; 
        int next_svc_status = global_stack.tcbs[schedule_thread].svc_status;
        set_svc_status(next_svc_status);
        return (void*)global_stack.tcbs[schedule_thread].msp;
    } else {
        return msp;
    }
}

/**
 * @brief  init thread function
 * @param max_threads max number of thread to be initialized
 * @param stack_size stack size for each thread
 * @param idle_fn idle function 
 * @param memory_protection specify memory protection type
 * @param max_mutexes max mutexes to be used
 * @return if successful, return 0; if not, return -1
 */
int sys_thread_init(uint32_t max_threads, uint32_t stack_size, void* idle_fn, mpu_mode memory_protection, uint32_t max_mutexes) {
    (void)max_threads; (void)stack_size; (void)idle_fn;
    (void)memory_protection; (void)max_mutexes;
    // make sure fit in these region
    uint32_t calcultaed_stack_size = 4 * stack_size;
    calcultaed_stack_size = (1 << mm_log2ceil_size(calcultaed_stack_size));
    if (calcultaed_stack_size * max_threads > STACK_SPACE_LIMIT) {
        return -1;
    }
    
    // initialize global variables
    global_stack.tcbs = TCB_array;
    global_stack.time = 0;
    global_stack.utilization = 0.0f;
    global_stack.max_threads_num = max_threads;
    global_stack.thread_count = 0;
    global_stack.thread_size = calcultaed_stack_size;
    // initialize current thread
    global_stack.current_thread = max_threads + 1;

    if (memory_protection == PER_THREAD) {
        global_stack.protection_flag = 1;
    } else {
        global_stack.protection_flag = 0;
    }

    for (uint32_t i = 0; i < 16; ++i) {
        global_stack.tcbs[i].status = UNINITIALIZE_STATUS;
    }

    // intialize mutex variable
    global_stack.mutexes = global_mutexes;
    global_stack.mutex_max = max_mutexes;
    global_stack.mutex_count = 0; 

    // if idle thread is NULL, create default idle thread
    if (idle_fn == NULL) {
        sys_thread_create((void*)default_idle, global_stack.max_threads_num, 0, 0, NULL);
    } else {
        sys_thread_create(idle_fn, global_stack.max_threads_num, 0, 0, NULL);
    }

    return 0;
}

/**
 * @brief  calculate utilization for UB test
 * @param C execution time (scheduler ticks)
 * @param T task period (scheduler ticks)
 * @return float, return calculated utilization
 */
float calculate_utilization(uint32_t C, uint32_t T) {
    return global_stack.utilization + (float)C / (float)T;
}

/**
 * @brief  create thread using provided parameters
 * @param fn function to be called, stored in pc register
 * @param prio priority for created thread
 * @param C execution time (scheduler ticks)
 * @param T task period (scheduler ticks)
 * @param vargp parameter for function, stored in ro register
 * @return if successful, return 0; else return -1
 */
int sys_thread_create(void* fn, uint32_t prio, uint32_t C, uint32_t T, void* vargp) {
    (void)fn; (void)prio; (void)C; (void)T; (void)vargp;
    // populate TCB, one new thread with new parameters, fill information 
    // where is initial msp, each has two stack

    TCB *tcb = &(global_stack.tcbs[prio]);

    if (tcb->status != UNINITIALIZE_STATUS) {
        return -1;
    }

    // ask why cannot cast    
    if (prio != global_stack.max_threads_num) {
        if (prio > global_stack.max_threads_num) {
            return -1;
        }

        if (C == 0 || T == 0) {
            return -1;
        }


        float new_utilization = calculate_utilization(C, T);
        if (new_utilization > ub_table[global_stack.thread_count + 1]) {
            return -1;
        }

        global_stack.utilization = new_utilization;
        global_stack.thread_count++;
        tcb->C = C;
        tcb->T = T;
    }

    tcb->thread_time = 0;
    tcb->sleep_time = T;

    // set svc_status
    tcb->svc_status = 0;
    // set thread status
    tcb->status = RUNNABLE_STATUS;
    // set prio
    tcb->priority = prio;

    msp_stack_frame* msp = (msp_stack_frame*)(&__thread_k_stacks_base - global_stack.thread_size * prio);
    msp = msp - 1;

    tcb->msp = msp;
    tcb->msp->lr = (uint32_t)LR_RETURN_TO_USER_PSP;

    
    interrupt_stack_frame* psp = (interrupt_stack_frame*)(&__thread_u_stacks_base - global_stack.thread_size * prio);
    tcb->psp_base = &__thread_u_stacks_base - global_stack.thread_size * (prio + 1);
    psp = psp - 1;

    tcb->msp->psp = (uint32_t)psp;

    psp->r0 = (uint32_t)vargp;
    psp->lr = (uint32_t)&thread_kill;
    psp->pc = (uint32_t)fn;

    psp->xPSR = XPSR_INIT;

    return 0;
}

/**
 * @brief  function for starting scheduler, start systick and call pendsv interrupt 
 * @param frequency freq for systick interrupt
 * @return if successful, return 0; else return -1
 */
int sys_scheduler_start(uint32_t frequency) {
    (void)frequency;
    // call systick start
    if (global_stack.thread_count == 0) {
        return -1;
    }

    if (global_stack.protection_flag == 1) {
        // per_thread
        mm_kernel_disable();
    } else {
        mm_kernel_enable(&__thread_u_stacks_limit, 32768);
    }

    systick_start(frequency);
    pend_pendsv();
    // what is bookkeeping
    // how to halt main thread

    return 0;
}

/**
 * @brief  round robin scheduler to find next thread to be scheduled
 * @return return next scheduled thread
 */
uint32_t round_robin_scheduler() {

    uint32_t schedule = global_stack.current_thread + 1;
    while (schedule != global_stack.current_thread) {
        schedule = schedule % global_stack.max_threads_num;
        if (global_stack.tcbs[schedule].status == RUNNABLE_STATUS) {
            return schedule;
        }
        schedule++;
    }
    return schedule;
}

/**
 * @brief  update RMS status for every systick
 * @return None
 */
void Systick_RMS_scheduler() {
    // 1. update computation time for the current thread:
    uint32_t curr_thread = global_stack.current_thread;
    TCB curr_tcb = global_stack.tcbs[curr_thread];
    curr_tcb.computation_time += 1; // reset the computation_time **
    curr_tcb.thread_time += 1;
    // 2. check if the computation time has reach to C:
    if (curr_tcb.computation_time >= curr_tcb.C) {
        // set it to waiting:
        curr_tcb.status = WAITING_STATUS;
        // wake up the thread
    }

    global_stack.tcbs[curr_thread] = curr_tcb;
    // 3. check any waiting thread need to be waken up and set it runnable
    uint32_t index = 0;
    while (index < global_stack.max_threads_num) {
        TCB cur = global_stack.tcbs[index];
        if (cur.status == UNINITIALIZE_STATUS) {
            index++;
            continue;
        }
        if (cur.sleep_time <= global_stack.time) {
            // set it runnable
            cur.status = RUNNABLE_STATUS;
            cur.computation_time = 0;
            // update next wake up time;
            cur.sleep_time = cur.sleep_time + cur.T;
        }
        global_stack.tcbs[index] = cur;
        index++;
    }

}

/**
 * @brief  RMS scheduler to find next thread to be scheduled
 * @return next scheduled thread
 */
uint32_t RMS_Scheduler_Highest() {
    uint32_t max = 0;
    while (max < global_stack.max_threads_num) {
        if (global_stack.tcbs[max].status == UNINITIALIZE_STATUS) {
            max++;
            continue;
        }
        if (global_stack.tcbs[max].waiting_mutex != NULL) {
            max++;
            continue;
        }
        if (global_stack.tcbs[max].status == RUNNABLE_STATUS) {
            return max;
        }
        max++;
    }
    // ??? return max? what to return if not found;
    return max;
}

/**
 * @brief  call corresponding scheduler function to find scheduled thread
 * @return next scheduled thread
 */
uint32_t schedule_next() {
    // first version: round robin
    // select the highest priority, sort the TCB array
    // return round_robin_scheduler();
    return RMS_PCP_Scheduler();
    // return RMS_Scheduler_Highest();
    // return (scheduled++ % thread_count);
}

/**
 * @brief  get thread priority
 * @return priority for current thread
 */
uint32_t sys_get_priority() {
    return global_stack.current_priority;
}

/**
 * @brief  get global time, update time field in every systick
 * @return global time when function is called 
 */
uint32_t sys_get_time() {
    return global_stack.time;
}

/**
 * @brief  get thread time
 * @return thread time when function is called
 */
uint32_t sys_thread_time() {
    // return 0;
    return global_stack.tcbs[global_stack.current_thread].thread_time;
}

/**
 * @brief  thread kill function
 * (1) kill current thread, update status
 * (2) update utilization field for thread reuse
 * @return None
 */
void sys_thread_kill() {
    // if idle and main thread then sys_exit
    if (global_stack.current_thread == global_stack.max_threads_num || global_stack.current_thread == global_stack.max_threads_num + 1) {
        sys_exit(-1);
    }

    // Kill cur thread
    global_stack.tcbs[global_stack.current_thread].status = UNINITIALIZE_STATUS;
    if (global_stack.current_thread != global_stack.max_threads_num) {
        global_stack.thread_count--;
    }

    
    // When no more thread:
    if (global_stack.thread_count == 0) {
        global_stack.tcbs[global_stack.max_threads_num].status = UNINITIALIZE_STATUS;
        global_stack.tcbs[global_stack.max_threads_num + 1].status = UNINITIALIZE_STATUS;
    } else {
        uint32_t C = global_stack.tcbs[global_stack.current_thread].C;
        uint32_t T = global_stack.tcbs[global_stack.current_thread].T;
        global_stack.utilization -= (float)C / (float)T;
    }


    pend_pendsv();
    return;
}

/**
 * @brief  syscall for wait until next period function
 * (1) update current thread's status to waiting status
 * (2) call pendsv to do context switch
 * @return None
 */
void sys_wait_until_next_period() {
    disable_interrupts();
    global_stack.tcbs[global_stack.current_thread].status = WAITING_STATUS;
    pend_pendsv();
    enable_interrupts();
}

/**
 * @brief  function for initializing mutex
 * @param max_prio update prio_ceil for mutex
 * @return initialized mutex structure
 */
kmutex_t* sys_mutex_init(uint32_t max_prio) {
    (void)max_prio;
    if (global_stack.mutex_count >= global_stack.mutex_max) {
        // mutexes too many
        return NULL;
    }

    // initialize mutex structure, and increase mutex count
    kmutex_t* get_mutex = &global_stack.mutexes[global_stack.mutex_count];
    get_mutex->locked_by = UNLOCKED;
    get_mutex->prio_ceil = max_prio;
    global_stack.mutex_count++;
    return (kmutex_t*)global_stack.mutex_count;
}

/**
 * @brief  check if mutex is locked by specific thread
 * @param mutex mutex to be checked
 * @return None
 */
void mutex_check_block(kmutex_t* mutex) {
    // if mutex is not blocked by any thread, update locked by using current thread
    if (mutex->locked_by == UNLOCKED) {
        mutex->locked_by = global_stack.current_thread;
        return;
    }
    // else, update waiting mutex
    TCB * get_tcb = &(global_stack.tcbs[global_stack.current_thread]);
    get_tcb->waiting_mutex = mutex;
    // call pendsv to context switch
    pend_pendsv();
}

/**
 * @brief  uunblock mutex, check next thread for this mutex
 * @param mutex mutex to be unblocked
 * @return None
 */
void mutex_check_unblock(kmutex_t* mutex) {
    // unblock current mutex
    mutex->locked_by = UNLOCKED;

    uint32_t index = 0;
    while(index < global_stack.max_threads_num) {
        TCB* get_tcb = &(global_stack.tcbs[index]);
        if (get_tcb->status == UNINITIALIZE_STATUS) {
            index++;
            continue;
        }
        if (get_tcb->waiting_mutex == mutex) {
            // update mutex locked by
            mutex->locked_by = index;
            pend_pendsv();
            return;
        }
        index++;
    }
}

/**
 * @brief  check if mutex exists
 * @param mutex_index mutex_index to be checked
 * @return return 0 if exists; return 1 if not exists
 */
uint32_t check_handle(uint32_t mutex_index) {
    if (mutex_index >= global_stack.mutex_count) {
        printk("mutex not exists\n");
        return 1;
    }
    return 0;
}

/**
 * @brief  function for locking mutex
 * (1) check if mutex is valid
 * (2) check main and idle thread
 * (3) check prio
 * (4) check repeat lock
 * @param mutex mutex to be locked
 * @return None
 */
void sys_mutex_lock(kmutex_t* mutex) {
    // (void)mutex;
    uint32_t mutex_index = (uint32_t)mutex - 1;
    // check if valid mutex handle
    if (check_handle(mutex_index) == 1) {
        return;
    }

    if (global_stack.current_thread == global_stack.max_threads_num || global_stack.current_thread == global_stack.max_threads_num + 1) {
        printk("main and idle thread cannot use mutexes\n");
        sys_exit(-1);
    }
    // main and idle thread cannot use mutexes
    kmutex_t* get_mutex = &(global_stack.mutexes[mutex_index]);

    // check prio with prio ceiling
    // if mutex priority too high, kill thread
    TCB *get_tcb = &global_stack.tcbs[global_stack.current_thread];

    // invalid higher priority
    if (get_tcb->priority < get_mutex->prio_ceil) {
        printk("invalid high priority\n");
        sys_thread_kill();
    }

    // check repeat lock
    if (get_mutex->locked_by == global_stack.current_thread) {
        printk("mutex repeat\n");
        return;
    }

    pcp_check_block(get_mutex);

}

/**
 * @brief  function for unlocking mutex
 * (1) check if mutex exists
 * (2) check if this mutex belong to current thread
 * @param mutex mutex to be unlocked
 * @return None
 */
void sys_mutex_unlock(kmutex_t* mutex) {
    (void)mutex;
    // if mutex is not blocked by current thread, print warning
    uint32_t mutex_index = (uint32_t)mutex - 1;

    if (check_handle(mutex_index) == 1) {
        return;
    }

    kmutex_t* get_mutex = &(global_stack.mutexes[mutex_index]);

    
    if (get_mutex->locked_by != global_stack.current_thread) {
        printk("this mutex doesn't belong to current thread");
        return;
    }
    // logic around the pcp protocol, update the lock status, invoke the pcp

    pcp_check_unblock(get_mutex);
}

/**
 * @brief  function for check current PCP
 * @param tcb tcb structure to be checked
 * @param index thread index
 * @return thread index
 */
uint32_t check_curr_PCP_NULL(TCB* tcb, uint32_t index) {
    tcb->waiting_mutex->locked_by = index;
    if (tcb->waiting_mutex->locked_by != UNLOCKED) {
        global_stack.current_PCP = tcb->waiting_mutex;
    }
    tcb->waiting_mutex = NULL;
    return index;
}

/**
 * @brief  function for check prio ceil
 * @param tcb tcb structure to be checked
 * @param index thread index
 * @return thread index
 */
uint32_t check_Prio_Ceil(TCB* tcb, uint32_t index) {
    tcb->waiting_mutex->locked_by = index;
    if (tcb->waiting_mutex->locked_by != UNLOCKED) {
        if (global_stack.current_PCP->prio_ceil > tcb->waiting_mutex->prio_ceil) {
            global_stack.current_PCP = tcb->waiting_mutex;
        }
    }
    tcb->waiting_mutex = NULL;
    return index;
}
/**
 * @brief  check valid condition
 * @param curr current thread index
 * @return true or false
 */
uint32_t check_valid(uint32_t curr) {
    return global_stack.current_PCP != NULL && global_stack.current_PCP->locked_by != curr && global_stack.current_PCP->prio_ceil <= curr;
}

/**
 * @brief  RMS scheduler with PCP
 * @return next thread to be scheduled
 */
uint32_t RMS_PCP_Scheduler() {
    uint32_t index = 0;
    while (index < global_stack.max_threads_num) {
        TCB* tcb = &global_stack.tcbs[index];
        if(tcb->status == UNINITIALIZE_STATUS) {
            index++;
            continue;
        }
        // update priority ceiling
        if(tcb->waiting_mutex != NULL) {
            global_stack.current_priority = index;
            if (check_valid(index)) {
                return global_stack.current_PCP->locked_by;
            }
            // update current_PCP if is NULL
            if(global_stack.current_PCP == NULL) {
                return check_curr_PCP_NULL(tcb, index);
            } else {
                return check_Prio_Ceil(tcb, index);
            }
        }
        // if status is RUNNABLE, update get_priority
        if(tcb->status == RUNNABLE_STATUS) {
            global_stack.current_priority = index;
            return index;
        }
        index++;
    }
    return index;
}

/**
 * @brief  function to check mutex block for pcp
 * @param mutex mutex to be checked
 * @return None
 */
void pcp_check_block(kmutex_t* mutex) {
    if(check_valid(global_stack.current_thread)) {
        // udpate waiting mutex
        global_stack.tcbs[global_stack.current_thread].waiting_mutex = mutex;
        // do context switch
        pend_pendsv();
    } else {
        // update locked_by to current thread
        mutex->locked_by = global_stack.current_thread;
        if (mutex->locked_by != UNLOCKED) {
            if (global_stack.current_PCP == NULL) {
                global_stack.current_PCP = mutex;
            }
            if (global_stack.current_PCP->prio_ceil > mutex->prio_ceil) {
                global_stack.current_PCP = mutex;
            }
        }
    }
    return;
}
/**
 * @brief  function to check mutex unblock for pcp
 * @param mutex mutex to be checked
 * @return None
 */
void pcp_check_unblock(kmutex_t* mutex) {
    mutex->locked_by = UNLOCKED;
    if (mutex == global_stack.current_PCP) {
        // unblock current_PCP, set to NULL
        global_stack.current_PCP = NULL;
        for(uint32_t mutex_index = 0; mutex_index < global_stack.mutex_count; mutex_index++) {
            kmutex_t* curr_mutex = &global_stack.mutexes[mutex_index];
            if (curr_mutex->locked_by != UNLOCKED) {
                if (global_stack.current_PCP == NULL) {
                    global_stack.current_PCP = curr_mutex;
                }
                if (global_stack.current_PCP->prio_ceil > curr_mutex->prio_ceil) {
                    global_stack.current_PCP = curr_mutex;
                }
            }
        }
    }
    pend_pendsv();
}