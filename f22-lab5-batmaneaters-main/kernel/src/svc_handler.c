/** @file   svc_handler.c
 *
 *  @brief  implementation of basic and custom SVC calls
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 25 October 2022
 *  @author CMU 14-642
**/

#include <arm.h>
#include <printk.h>
#include <svc_num.h>
#include <unistd.h>
#include <stdint.h>
#include <syscall_mutex.h>
#include <syscall_thread.h>
#include <syscall.h>
#include <timer.h>
#include <mpu.h>

/**
 * @brief define stack_frame_t
**/
typedef struct {
  uint32_t r0; /**< r0 register */
  uint32_t r1; /**< r1 register */
  uint32_t r2; /**< r2 register */
  uint32_t r3; /**< r3 register */
  uint32_t r12; /**< r12 register */
  uint32_t lr; /**< lr register */
  uint32_t pc; /**< pc register */
  uint32_t xPSR; /**< xPSR register */
  uint32_t arg5; /**< arg5 register */
} stack_frame_t;

/**
 * @brief Function svc_c_handler, handling different svc number
 * (1) this function is used for get svc_num from pc register, 2 bytes step from pc register
 * (2) call corresponding syscall using the svc number
 * (3) using r0 as the return register to return the result from syscall
 * @param psp psp register, used for get svc_num
 * @return None
**/
void svc_c_handler(void* psp) {
  // find which handler to call
  stack_frame_t* s = (stack_frame_t*)psp;
  uint8_t* pc = (uint8_t*)s->pc;
  // get svc_num from pc register
  uint8_t svc_num = pc[-2];
  switch(svc_num) {
    case SVC_START_FLASH:
        int freq_flash = (int)s->r0;
        sys_start_flash(freq_flash);
        break;
    case SVC_STOP_FLASH:
        sys_stop_flash();
        break;
    case SVC_VISUAL_START:
        sys_visual_start();
        break;

    case SVC_VISUAL_STOP:
        sys_visual_stop();
        break;

    case SVC_RANDOM:
        s->r0 = (uint8_t)sys_random_initial();
        break;
    case SVC_BUTTON:
        s->r0 = (uint32_t)sys_button_press();
        break;
    case SVC_THR_INIT:
        // cast register to argument value
        uint32_t max_threads = (uint32_t)s->r0;
        uint32_t stack_size = (uint32_t)s->r1;
        void* idle_fn = (void*)s->r2;
        mpu_mode memory_protection = (mpu_mode)s->r3;
        // how to deal with arguments
        uint32_t max_mutexes = (uint32_t)s->arg5;
        // uint32_t max_mutexes = (uint32_t)s->r12;
        // how to return result ???
        int init_result = sys_thread_init(max_threads, stack_size, idle_fn, memory_protection, max_mutexes);
        s->r0 = (uint32_t)init_result;
        break;
    
    case SVC_THR_CREATE:
        void* fn = (void*)s->r0;
        uint32_t prio = (uint32_t)s->r1;
        uint32_t C = (uint32_t)s->r2;
        uint32_t T = (uint32_t)s->r3;
        // how to deal with arguments
        void* vargp = (void*)s->arg5;
        // void* vargp = (void*)s->r12;
        // return result
        int create_result = sys_thread_create(fn, prio, C, T, vargp);
        s->r0 = (uint32_t)create_result;
        break;

    case SVC_THR_KILL:
        sys_thread_kill();
        break;

    case SVC_SCHD_START:
        uint32_t freq = (uint32_t)s->r0;
        // return result
        int scheduler_result = sys_scheduler_start(freq);
        s->r0 = (uint32_t)scheduler_result;
        break;

    case SVC_PRIORITY:
        // return result
        s->r0 = sys_get_priority();
        break;

    case SVC_THR_TIME:
        // return result
        s->r0 = sys_thread_time();
        break;

    case SVC_WAIT:
        sys_wait_until_next_period();
        break;

    case SVC_TIME:
        s->r0 = sys_get_time();
        break;

    case SVC_MUT_INIT:
        uint32_t max_prio = (uint32_t)s->r0;
        //return result
        kmutex_t* mutex_result = sys_mutex_init(max_prio);
        // ?? return result
        s->r0 = (uint32_t)mutex_result;
        break;

    case SVC_MUT_LOK:
        kmutex_t* mutex_lock = (kmutex_t*)s->r0;
        sys_mutex_lock(mutex_lock);
        break;

    case SVC_MUT_ULK:
        kmutex_t* mutex_unlock = (kmutex_t*)s->r0;
        sys_mutex_unlock(mutex_unlock);
        break;

    case SVC_PIX:
      uint8_t red = (uint8_t)s->r0;
      uint8_t green = (uint8_t)s->r1;
      uint8_t blue = (uint8_t)s->r2;
      sys_pix_set(red, green, blue);
      break;

    case SVC_SBRK:
      int incr = (int)s->r0;
      void* return_pointer = sys_sbrk(incr);
      s->r0 = (uint32_t)return_pointer;
      break;
      
    case SVC_WRITE:
      int write_file = (int)s->r0;
      char* write_ptr = (char*)s->r1;
      int write_len = (int)s->r2;
      uint32_t write_return_value = (uint32_t)sys_write(write_file, write_ptr, write_len);
      s->r0 = write_return_value;
      break;

    case SVC_READ:
      int read_file = (int)s->r0;
      char *read_ptr = (char*)s->r1;
      int read_len = (int)s->r2;
      uint32_t return_value = (uint32_t)sys_read(read_file, read_ptr, read_len);
      s->r0 = return_value;
      break;

    case SVC_EXIT:
      int status = (int)s->r0;
      // systick_stop();
      sys_exit(status);
      break;

    case SVC_FSTAT:
      s->r0 = -1;
      break;

    case SVC_ISATTY:
      s->r0 = 1;
      break;

    case SVC_LSEEK:
      s->r0 = -1;
      break;
      
    default:
      printk("Not implemented, svc num %d\n", svc_num);
      breakpoint();
  }
}
