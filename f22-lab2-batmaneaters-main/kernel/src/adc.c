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
/**
 * @brief global variable used for storing result
**/
int16_t result;

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
    // A1 AIN3 
    uint32_t channel_offset = 0x518 + 3 * 0x10;
    
    uint32_t *channel_offset_address = (uint32_t *)(base_address + channel_offset);
    // configure gain1_4
    *channel_offset_address |= (2 << 8);
    // configure internal reference
    *channel_offset_address &= ~(1 << 12);

    uint32_t *resolution_offset_address = (uint32_t *)(base_address + 0x5F0);
    // configure resolution
    *resolution_offset_address = 2;


    // configure result register
    uint32_t result_offset_address = base_address + 0x62C;
    *(uint32_t *)result_offset_address = (uint32_t)&result;
    
    // uint32_t get_result = (uint32_t)result;
    uint32_t *result_count_address = (uint32_t *)(base_address + 0x630);
    *result_count_address = 1;


    // enable SAADC
    uint32_t *enable_offset_address = (uint32_t *)(base_address + 0x500);
    *enable_offset_address = 1;

    uint32_t pin_offset = 0x510 + 3 * 0x10;
    uint32_t *pin_offset_address = (uint32_t *)(base_address + pin_offset);
    *pin_offset_address = 4;


    return;
}


/**
 * @brief Function for get value from the microphone, we do following things in the below function:
 * (1) trigger and start task
 * (2) trigger task sample to let adc get sound sample
 * (3) make sure the event is done before returning corresponding value
 * @return Value get from the adc and return it as int16_t
**/
int16_t adc_read_pin() {
    // start the task
    uint32_t base_address = 0x40007000;
    
    // task_start
    uint32_t *task_start_address = (uint32_t *)base_address; //offset is 0x000
    *task_start_address |= 1; // trigger task


    uint32_t *event_start_address = (uint32_t *)(base_address + 0x100);
    while(1) {
        uint8_t bit_value = *event_start_address & 1;
        if (bit_value == 1) {
            break;
        }
    }

    // take one SAADC sample
    uint32_t *task_sample_address = (uint32_t *)(base_address + 0x004);
    *task_sample_address |= 1;

    // event result done
    uint32_t *event_result_end = (uint32_t *)(base_address + 0x104);
    while(1) {
        uint8_t bit_value = *event_result_end & 1;
        if (bit_value == 1) {
            break;
        }
    }

    return result;
}