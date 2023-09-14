/** @file   adc.c
 *
 *  @brief  ADC driver implementation
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 20 September 2022
 *  @author xingzhi2, ruobing2
**/

#include <adc.h>
#include <stdint.h>
#include <visualizer.h>
#include <printk.h>
// #include <string.h>
/**
 * @brief global variable used for storing result
**/
int16_t result[256];


/**
 * @brief Function used for adc initialization, we initialize below components:
 * (1) configure the gain, internal reference, acquisition time and mode
 * (2) configure resolution for adc which is 12-bit mode
 * (3) configure result pointer
 * (4) configure pin selection
 * (5) configure result amount and enable adc at last
 * @return None
**/
void adc_init() {
    uint32_t base_address = 0x40007000;

    // set resolution to 12 bit
    uint32_t *resolution_address = (uint32_t*)(base_address + 0x5F0);
    *resolution_address = 0x2;

    // set AIN0
    uint32_t *pselp_address = (uint32_t*)(base_address + 0x510);
    *pselp_address = 0x1;

    // intenset for event start
    uint32_t *intenset = (uint32_t*)(base_address + 0x304);
    *intenset |= 0x1;

    // intenset for event end
    *intenset |= (0x1 << 1);

    // set nvic
    uint32_t *nvic_iser = (uint32_t*)0xE000E100;
    *nvic_iser |= (0x1 << 7);

    // enable adc
    uint32_t *enable_address = (uint32_t*)(base_address + 0x500);
    *enable_address = 0x1;


    return;
}

/**
 * @brief Function for get value from the microphone, we do following things in the below function:
 * (1) trigger and start task
 * (2) trigger task sample to let adc get sound sample
 * (3) make sure the event is done before returning corresponding value
 * @return Value get from the adc and return it as int16_t
**/
void adc_read_samples(int16_t* output) {
    // start the task
    
    uint32_t base_address = 0x40007000;

    // set result ptr
    uint32_t *result_address = (uint32_t*)(base_address + 0x62C);
    *result_address = (uint32_t)output;

    // set max number of result buffer
    uint32_t *result_max_address = (uint32_t*)(base_address + 0x630);
    *result_max_address = 256;

    uint32_t *result_start_address = (uint32_t*)base_address;
    *result_start_address = 1;

    return ;
}



