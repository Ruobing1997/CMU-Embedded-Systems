/** @file   rtt.h
 *
 *  @brief  structs, defines, and prototypes for RTT
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 24 October 2022
 *  @author Xingzhi Wang
**/

#ifndef _USER_SW_H_
#define _USER_SW_H_
#include <stdint.h>
/**
 * @brief initializing user button
**/
void enable_userSw();

/**
 * @brief listening from user button
**/
uint8_t UserSw_event();

#endif



