/** @file   pix.c
 *
 *  @brief  pwm driver for ws2812b "neopixel"
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 20 September 2022
 *  @author xingzhi2, ruobing2
**/


#include <arm.h>
#include <pix.h>
#include <gpio.h>
#include <stdint.h>



/** up the GPIO voltage for the Neopixel and set up the PWM module */
/**
 * @brief Global array variable used for color setting 
**/
uint16_t arr[100];

/**
 * @brief compare value for 0
**/
int comp0 = 6;
/**
 * @brief compare value for 1
**/
int comp1 = 13;
/**
 * @brief polarity_mask used for setting
**/
int polarity_mask = (1 << 15);

/**
 * @brief Function used for initializing neopixel
 * (1) first initialize specific gpio pin
 * (2) and then select the corresponding pin
 * (3) configure the countertop value
 * (4) enable the PWM
 * @return None
**/
void pix_init() {
    /*
     * the neopixel needs to have input voltage of 3.3V
     * UICR and NVMC details are set here based on values in pix.h
     */
    if(UICR_REG0 != 5UL) {
        NVMC_CONFIG = 1UL;
        BUSY_LOOP(!NVMC_READY);
        UICR_REG0 = 5UL;
        NVMC_CONFIG = 0UL;
        BUSY_LOOP(!NVMC_READY);
    }
    uint32_t base_address = 0x4001C000;

    // gpio init
    gpio_init(0, 16, MODE_OUTPUT, OUTPUT_S0H1, PUPD_PULL_DOWN);

    // PSEL.OUT[0] for gpio pin
    uint32_t *psel_offset_address = (uint32_t *)(base_address + 0x560);
    // pin
    *psel_offset_address = 0x10;
    // port
    *psel_offset_address &= ~(1 << 5);
    // connect
    *psel_offset_address &= ~(1 << 31);

    // countertop offset address
    // countertop 20
    uint32_t *countertop_offset_address = (uint32_t *)(base_address + 0x508);
    *countertop_offset_address = 0x14;

    // enable PWM
    uint32_t *enable_offset_address = (uint32_t *)(base_address + 0x500);
    *enable_offset_address = 1;


}

/**
 * @brief Function used for setting neopixel value
 * @param r: value for r
 * @param g: value for g
 * @param b: value for b
 * @return None
**/
void pix_color_set(uint8_t r, uint8_t g, uint8_t b) {
    (void)r;
    (void)g;
    (void)b;
    // buffer for 24 bit rgb
    // uint8_t rgb_buf[3] = {};
    // rgb_buf[0] = r;
    // uint32_t rgb_buf;
    // rgb_buf = (r << 16) | (g << 8) | b;
    int index = 0;

    // loop 8 digits for g:
    for (int i = 7; i >= 0; --i)
    {
        if (g & (1 << i))  // if bit i is set
        {
            // add comp1 to array
            arr[index++] = comp1;

        } else {
            // add comp0 to array
            arr[index++] = comp0;
        }
    }

    // loop 8 digits for g:
    for (int i = 7; i >= 0; --i)
    {
        if (r & (1 << i))  // if bit i is set
        {
            // add comp1 to array
            arr[index++] = comp1;

        } else {
            // add comp0 to array
            arr[index++] = comp0;
        }
    }

    // loop 8 digits for b:
   for (int i = 7; i >= 0; --i)
    {
        if (b & (1 << i))  // if bit i is set
        {
            // add comp1 to array
            arr[index++] = comp1;

        } else {
            // add comp0 to array
            arr[index++] = comp0;
        }
    }

    // now the array should have 24 values for compare values
    // add polarity:
    for (int i = 0; i < 24; i++) {
        arr[i] = arr[i] | polarity_mask;
    }

    for (int i = 25; i < 100; i++) {
        arr[i] = 40;
    }

    // breakpoint();
    uint32_t base_address = 0x4001C000;

    // SEQ[0].refresh
    uint32_t *seq0_refresh_offset_address = (uint32_t *)(base_address + 0x528);
    *seq0_refresh_offset_address = 0;

    // SEQ[0].PTR offset address 
    uint32_t seq0_ptr_offset_address = (base_address + 0x520);
    *(uint32_t *)seq0_ptr_offset_address = (uint32_t) arr;

    // SEQ[0].CNT
    uint32_t *seq0_cnt_offset_address = (uint32_t *)(base_address + 0x524);
    *seq0_cnt_offset_address = 0x64;

    // SEQ[0] task start
    uint32_t *seq0_task_offset_address = (uint32_t *)(base_address + 0x008);
    *seq0_task_offset_address = 1;

    // SEQ[0] event end
    uint32_t *seq0_eventEnd_offset_address = (uint32_t *)(base_address + 0x110);
    while(1) {
        uint8_t bit_value = *seq0_eventEnd_offset_address & 1;
        if (bit_value == 1) {
            break;
        }
    }
}

