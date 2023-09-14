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

    // uint32_t result_offset_address = base_address + 0x62C;
    // *(uint32_t *)result_offset_address = (uint32_t)result;
    
    // uint32_t get_result = (uint32_t)result;
    // configure maxcount
    uint32_t *max_count_address = (uint32_t *)(base_address + 0x630);
    *max_count_address = 0x100;
    

    // enable intenset
    // enable event end interrupt
    uint32_t *intenset_address = (uint32_t *)(base_address + 0x304);
    *intenset_address = (0x2);

    // ? reach maxcount and issue an interrupt handler
    // enable adc interrupt handler
    uint32_t* nvic_iser = (uint32_t *)0xE000E100;
    *nvic_iser |= (1 << 7);


    // enable SAADC
    uint32_t *enable_offset_address = (uint32_t *)(base_address + 0x500);
    *enable_offset_address = 1;

    uint32_t pin_offset = 0x510 + 3 * 0x10;
    uint32_t *pin_offset_address = (uint32_t *)(base_address + pin_offset);
    *pin_offset_address = 4;

    // task_start
    uint32_t *task_start_address = (uint32_t *)base_address; //offset is 0x000
    *task_start_address |= 1; // trigger task


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
    (void) output;
    
    // start next task
    // uint32_t *task_start_address = (uint32_t *)0x40007000; //offset is 0x000
    // *task_start_address |= 1; // trigger task

    uint32_t result_offset_address = 0x4000762C;
    *(uint32_t *)result_offset_address = (uint32_t)output;


    uint32_t *task_stop_address = (uint32_t *)0x40007008;
    *task_stop_address |= 1;


    uint32_t *event_stop_address = (uint32_t *)0x40007114;
    while(1) {
        uint8_t bit_value = *event_stop_address & 1;
        if (bit_value == 1) {
            break;
        }
    }

    uint32_t *event_end_address = (uint32_t *)(0x40007104);
    *event_end_address = 0;

    uint32_t *task_start_address = (uint32_t *)0x40007000; //offset is 0x000
    *task_start_address |= 1; // trigger task
    


    return ;
}



