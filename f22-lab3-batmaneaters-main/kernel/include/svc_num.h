/**
 * @file   svc_num.h
 *
 * @brief  SVC numbers used by kernel and newlib syscalls
 *
 * @date   October 6, 2022
 *
 * @author CMU 14-642
 */

#ifndef _SVC_NUM_H_
#define _SVC_NUM_H_

/** @brief SVC number for sbrk() */
#define SVC_SBRK            0
/** @brief SVC number for write() */
#define SVC_WRITE           1
/** @brief SVC number for close() */
#define SVC_CLOSE           2
/** @brief SVC number for fstat() */
#define SVC_FSTAT           3
/** @brief SVC number for isatty() */
#define SVC_ISATTY          4
/** @brief SVC number for lseek() */
#define SVC_LSEEK           5
/** @brief SVC number for read() */
#define SVC_READ            6
/** @brief SVC number for exit() */
#define SVC_EXIT            7



/** custom syscalls for lab 3 user-space apps */
/** @brief SVC number for delay_ms() */
#define SVC_DELAY_MS        22
/** @brief SVC number for lux_read() */
#define SVC_LUX             23
/** @brief SVC number for pix_set() */
#define SVC_PIX             24



#endif /* _SVC_NUM_H_ */
