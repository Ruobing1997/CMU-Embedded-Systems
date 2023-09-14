/** @file   svc_num.h
 *
 *  @brief  constant defines for svc calls for lab 4
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 24 October 2022
 *  @author CMU 14-642
**/

#ifndef _SVC_NUM_H_
#define _SVC_NUM_H_

/** @brief SVC number for sbrk() */
#define SVC_SBRK    0
/** @brief SVC number for write() */
#define SVC_WRITE   1
/** @brief SVC number for close() */
#define SVC_CLOSE   2
/** @brief SVC number for fstat() */
#define SVC_FSTAT   3
/** @brief SVC number for isatty() */
#define SVC_ISATTY  4
/** @brief SVC number for lseek() */
#define SVC_LSEEK   5
/** @brief SVC number for read() */
#define SVC_READ    6
/** @brief SVC number for exit() */
#define SVC_EXIT    7
/** @brief SVC number for sys_kill() */
#define SVC_KILL    8
/** @brief SVC number for thread_init() */
#define SVC_THR_INIT    9
/** @brief SVC number for thread_create() */
#define SVC_THR_CREATE  10
/** @brief SVC number for thread_kill() */
#define SVC_THR_KILL  11
/** @brief SVC number for get_pid() */
#define SVC_GET_PID  12
/** @brief SVC number for mutex_init() */
#define SVC_MUT_INIT    13
/** @brief SVC number for mutex_lock() */
#define SVC_MUT_LOK     14
/** @brief SVC number for mutex_unlock() */
#define SVC_MUT_ULK     15
/** @brief SVC number for wait_until_next_period() */
#define SVC_WAIT        16
/** @brief SVC number for get_time() */
#define SVC_TIME        17
/** @brief SVC number for scheduler_start() */
#define SVC_SCHD_START  18
/** @brief SVC number for get_priority() */
#define SVC_PRIORITY    19
/** @brief SVC number for thread_get_time() */
#define SVC_THR_TIME   20
/** @brief SVC number for sleep_till_interrupt */
#define SVC_SLEEP_TILL_INT 21

/** custom syscalls for lab 3 user-space apps */
/** @brief SVC number for delay_ms() */
#define SVC_DELAY_MS        22
/** @brief SVC number for lux_read() */
#define SVC_LUX             23
/** @brief SVC number for pix_set() */
#define SVC_PIX             24
/** @brief SVC number for button_press() */
#define SVC_BUTTON          25
/** @brief SVC number for random_initial() */
#define SVC_RANDOM          26
/** @brief SVC number for visualizer_start() */
#define SVC_VISUAL_START    27
/** @brief SVC number for visualizer_stop() */
#define SVC_VISUAL_STOP     28
/** @brief SVC number for start_flash() */
#define SVC_START_FLASH       29
/** @brief SVC number for stop_flash() */
#define SVC_STOP_FLASH       30

#endif /* _SVC_NUM_H_ */
