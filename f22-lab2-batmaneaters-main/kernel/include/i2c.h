/** @file   i2c.h
 *
 *  @brief  Prototypes for I2C setup, read, and write
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 05 September 2022
 *  @author CMU 14-642
**/
#ifndef _I2C_H_
#define _I2C_H_

#include <unistd.h>
#include <stdint.h>

void i2c_leader_init();
int i2c_leader_write(uint8_t *tx_buf, uint8_t tx_len, uint8_t follower_addr);
uint16_t i2c_leader_read(uint8_t *rx_buf, uint8_t rx_len, uint8_t follower_addr);
void i2c_leader_stop();

#endif /* _I2C_H_ */
