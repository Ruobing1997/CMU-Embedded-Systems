#include <arm.h>
#include <printk.h>
#include <svc_num.h>
#include <unistd.h>
#include <stdint.h>
#include <syscall.h>

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
    case SVC_DELAY_MS:
      uint32_t ms = s->r0;
      sys_delay_ms(ms);
      break;
    
    case SVC_LUX:
      uint16_t result_read = sys_lux_read();
      s->r0 = (uint32_t)result_read;
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
      int write_return_value = (uint32_t)sys_write(write_file, write_ptr, write_len);
      s->r0 = write_return_value;
      break;

    case SVC_READ:
      int read_file = (int)s->r0;
      char *read_ptr = (char*)s->r1;
      int read_len = (int)s->r2;
      int return_value = (uint32_t)sys_read(read_file, read_ptr, read_len);
      s->r0 = return_value;
      break;

    case SVC_EXIT:
      int status = (int)s->r0;
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
