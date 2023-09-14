/** @file   i2c.c
 *
 *  @brief  I2C driver implementation
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 20 September 2022
 *  @author xingzhi2, ruobing2
**/

#include <i2c.h>
#include <gpio.h>
#include <arm.h>
#include <stdint.h>

/**
 * @brief Function used for i2c leader initialization, we initialize below components:
 * (1) initialize specific physical pin, p0.11 and p0.12
 * (2) configure SCL and SDA
 * (3) enable TWI
 * @return None
**/

void i2c_leader_init(){
    uint32_t TWIM_base_address = 0x40003000;

    // gpio SCL physical pin, p0, 11
    gpio_init(0, 11, MODE_INPUT, 7, PUPD_PULL_UP);
    // gpio SDA physical pin, p0, 12
    gpio_init(0, 12, MODE_INPUT, 7, PUPD_PULL_UP);

    // configure SCL
    uint32_t *SCL_offset_address = (uint32_t *)(TWIM_base_address + 0x508);
    // SCL pin
    *SCL_offset_address = 0xb;
    // SCL port 
    *SCL_offset_address &= ~(1 << 5);
    // SCL connect
    *SCL_offset_address &= ~(1 << 31);

    // configure SDA
    uint32_t *SDA_offset_address = (uint32_t *)(TWIM_base_address + 0x50C);
    // SDA pin
    *SDA_offset_address = 0xc;
    // SDA pin
    *SDA_offset_address &= ~(1 << 5);
    // SDA connect
    *SDA_offset_address &= ~(1 << 31);

    
    // enable TWI
    uint32_t *enable0_offset_address = (uint32_t *)(TWIM_base_address + 0x500);
    *enable0_offset_address = 6;

    return;
}

/**
 * @brief Function used for stopping i2c task
 * (1) first we will issue a stop task
 * (2) and then we will wait for specific event to see whether the task completed or not
 * @return None
**/
void i2c_leader_stop() {

    uint32_t base_address = 0x40003000;

    // stop task started
    uint32_t *stop_offset_address = (uint32_t *)(base_address + 0x014);
    *stop_offset_address = 1;

    // receive event stop
    uint32_t *event_stop_address = (uint32_t *)(base_address + 0x104);
    while(1) {
        uint8_t bit_value = *event_stop_address & 1;
        if (bit_value == 1) {
            break;
        }
    }

    return;
}

/**
 * @brief Function for i2c leader write
 * (1) we need to configure the follower address and then configure the txd maxcnt and txd data pointer
 * (2) and then we will start the txd task and then wait for the last byte event
 * @param tx_buf: store bytes that need to be written
 * @param tx_len: buffer length for bytes written
 * @param follower_addr: follower address needed to be configured
 * @return return type int. Using this value to see whether the write function is successful or not
**/
// type int?
int i2c_leader_write(uint8_t *tx_buf, uint8_t tx_len, uint8_t follower_addr) {
    (void)tx_buf;
    (void)tx_len;
    (void)follower_addr;
    
    uint32_t base_address = 0x40003000;

    // configure follower address
    uint32_t *follower_address = (uint32_t *)(base_address + 0x588);
    *follower_address = (uint32_t)follower_addr;

    // configure txd register
    uint32_t *txd_maxcnt = (uint32_t *)(base_address + 0x548);
    *txd_maxcnt = tx_len;

    // configure txd data pointer
    // TXD.PTR offset 0x544
    uint32_t txd_data_address = base_address + 0x544;
    *(uint32_t *)txd_data_address = (uint32_t)tx_buf;



    // start txd task
    uint32_t *txd_offset_address = (uint32_t *)(base_address + 0x008);
    *txd_offset_address = 1;

    // event started
    uint32_t *event_txstart_address = (uint32_t *)(base_address + 0x150);
    while(1) {
        uint8_t bit_value = *event_txstart_address & 1;
        if (bit_value == 1) {
            break;
        }
    }

    // event start to receive last byte
    uint32_t *event_lasttx_address = (uint32_t *)(base_address + 0x160);
    while(1) {
        uint8_t bit_value = *event_lasttx_address & 1;
        if (bit_value == 1) {
            break;
        }
    }

    return 0;
}

/**
 * @brief Function used for i2c leader read
 * (1) we will first configure the follower address and configure the result data pointer
 * (2) and then we start the receive task, waiting for the event to see whether the task completed or not
 * @param rx_buf: store result in this rx_buf
 * @param rx_len: length for result buffer
 * @param follower_addr: follower address needed to be configured
 * @return None
**/
// type int?
int i2c_leader_read(uint8_t *rx_buf, uint8_t rx_len, uint8_t follower_addr) {
    (void)rx_buf;
    (void)rx_len;
    (void)follower_addr;

    uint32_t base_address = 0x40003000;

    // configure follower address
    uint32_t *follower_address = (uint32_t *)(base_address + 0x588);
    *follower_address = (uint32_t)follower_addr;

    // configure rxd data pointer
    uint32_t rxd_data_address = (base_address + 0x534);
    *(uint32_t *)rxd_data_address = (uint32_t)rx_buf;


    // configure rxd register
    uint32_t *rxd_maxcnd = (uint32_t *)(base_address + 0x538);
    // maximum number of bytes in receive buffer
    *rxd_maxcnd = rx_len;


    // start receive task
    uint32_t *start_offset_address = (uint32_t *)(base_address);
    *start_offset_address = 1;

    // event start to receive last bytes
    uint32_t *event_lastrx_address = (uint32_t *)(base_address + 0x15C);
    while(1) {
        uint8_t bit_value = *event_lastrx_address & 1;
        if (bit_value == 1) {
            break;
        }
    }

    return rx_buf[1] << 8 | rx_buf[0];
}
