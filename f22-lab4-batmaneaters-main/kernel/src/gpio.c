/** @file   gpio.c
 *
 *  @brief  GPIO interface implementation
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 20 September 2022
 *  @author CMU 14-642
**/

#include <gpio.h>
#include <stdint.h>

/** 
 * @brief gpio_init function is used for initializing 
 * (1) we should first find whether the port is 0 or 1
 * (2) then we need to configure mode, output_type and pupd
 * @param port: port to be initialized
 * @param pin: pin to be initialized
 * @param mode: mode for initialization
 * @param otype: output type for initialization
 * @param pupd: setting for pulling up or pulling down
 * @return None
**/
void gpio_init(uint8_t port, uint8_t pin, uint8_t mode, uint8_t otype, uint8_t pupd) {
    (void)port;
    (void)pin;
    (void)mode;
    (void)otype;
    (void)pupd;
    uint32_t offset = 0x700 + pin * 0x4;
    uint32_t port_base = 0;
    if (port == 0) {
        port_base = 0x50000000;
    } else if (port == 1) {
        port_base = 0x50000300;
    }
    uint32_t *offset_address = (uint32_t *)(port_base + offset);
    // configure mode
    // *offset_address |= (1 << mode);
    *offset_address |= mode;
    // configure otype
    // *offset_address |= (1 << otype);
    *offset_address |= (otype << 8);
    // configure pupd
    *offset_address |= (pupd << 2);
    
} 

/** 
 * @brief gpio_set function is used for initializing 
 * (1) we should first find whether the port is 0 or 1
 * (2) set port/pin to high
 * @param port: set value to port
 * @param pin: set value to pin
 * @return None
**/
void gpio_set(uint8_t port, uint8_t pin) {
    (void)port;
    (void)pin;
    uint32_t port_base = 0;
    uint32_t offset = 0x508;
    if (port == 0) {
        port_base = 0x50000000;
    } else if (port == 1) {
        port_base = 0x50000300;
    }
    uint32_t *offset_address = (uint32_t *)(port_base + offset);
    // set port/pin to high
    *offset_address = (1 << pin);

}

/** 
 * @brief gpio_clr function is clear gpio setting
 * (1) we should first find whether the port is 0 or 1
 * (2) set port/pin to low
 * @param port: port to be cleared
 * @param pin: pin to be cleared
 * @return None
**/
void gpio_clr(uint8_t port, uint8_t pin) {
    (void)port;
    (void)pin;
    uint32_t port_base = 0;
    uint32_t offset = 0x50C;
    if (port == 0) {
        port_base = 0x50000000;
    } else if (port == 1) {
        port_base = 0x50000300;
    }
    uint32_t *offset_address = (uint32_t *)(port_base + offset);
    // set port/pin to low
    *offset_address = (1 << pin);
}

/** 
 * @brief gpio_read_all function is used for read all pin result
 * @param port: specific port to read from
 * @return value for all pin, return type is uint32_t
**/
uint32_t gpio_read_all(uint8_t port) {
    (void)port;
    uint32_t port_base = 0;
    // offset for GPIO IN
    uint32_t offset = 0x510;
    if (port == 0) {
        port_base = 0x50000000;
    } else if (port == 1) {
        port_base = 0x50000300;
    }
    uint32_t *offset_address = (uint32_t *)(port_base + offset);
    
    return *offset_address;
}

/** 
 * @brief gpio_read function is used for reading specific pin result
 * (1) we should first find whether the port is 0 or 1
 * (2) get specific pin value
 * @param port: port for pins
 * @param pin: specific pin to read
 * @return return specific pin result value, return type is uint8_t
**/
uint8_t gpio_read(uint8_t port, uint8_t pin) {
    (void)port;
    (void)pin;
    uint32_t port_base = 0;
    // offset for GPIO IN
    uint32_t offset = 0x510;
    if (port == 0) {
        port_base = 0x50000000;
    } else if (port == 1) {
        port_base = 0x50000300;
    }
    uint32_t *offset_address = (uint32_t *)(port_base + offset);
    uint8_t bit_value = *offset_address & (1 << pin);


    return bit_value;
}

