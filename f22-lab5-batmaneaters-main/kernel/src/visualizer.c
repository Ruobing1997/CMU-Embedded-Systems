/** @file   visualizer.c
 *
 *  @brief  implementation of color visualizer
 *  @note   Not for public release, do not share
 *
 *  @date   last modified 11 October 2022
 *  @author CMU 14-642
**/

#include <rfft.h>
#include <unistd.h>
#include <visualizer.h>
#include <adc.h>
#include <timer.h>
#include <printk.h>
#include <pix.h>
#include <adc.h>
#include <stdbool.h>

#define R_LIM   (85)
#define G_LIM   (170)
#define RGB_MAX (255)
#define SCALE_FACT (4)

static int16_t input[FFT_SIZE];     // FFT_SIZE real values
static int16_t output[FFT_SIZE*2];  // FFT_SIZE complex (real,imag) values
static int16_t mags[FFT_SIZE];      // fft output magnitudes
static uint8_t r, g, b;
static bool conv_enb = false;

/**
 * @brief initialize visualizer
 * @return None
**/
void visualizer_init() {
  // maybe some things to initialize here...
  start_adc_timer();
  adc_read_samples(input);
  
  return;
}

/* similar to previous visualizer_color_info, mod'd to use global variables */
void visualizer_color_info() {
    uint32_t r_avg = 0, g_avg = 0, b_avg = 0;
    uint16_t i;
    rfft(input, output);
    rfft_abs(output, mags, FFT_SIZE);
    
    r_avg = g_avg = b_avg = 0;
    for(i = 2; i < R_LIM; i++)
        r_avg += mags[i];
    for(; i < G_LIM; i++)
        g_avg += mags[i];
    for(; i < FFT_SIZE; i++)
        b_avg += mags[i];

    r_avg >>= SCALE_FACT;
    g_avg >>= SCALE_FACT;
    b_avg >>= SCALE_FACT;
    r = r_avg > RGB_MAX ? RGB_MAX : r_avg;
    g = g_avg > RGB_MAX ? RGB_MAX : g_avg;
    b = b_avg > RGB_MAX ? RGB_MAX : b_avg;
    
    // anything else you'd like to do here?
    
    return;
}

/**
 * @brief timer 0 handler for adc
 * @return None
**/
void TIMER0_IRQHandler() {
    uint32_t *timer0_event_compare = (uint32_t*)0x40008140;
    if(*timer0_event_compare) {
        if (conv_enb) {
            uint32_t *sample_address = (uint32_t*)0x40007004;
            *sample_address = 0x1;
        }
        *timer0_event_compare = 0x0;
        uint32_t *timer0_clear_address = (uint32_t*)0x4000800C;
        *timer0_clear_address = 0x1;
    }
}

/**
 * @brief interrupt handler for adc 
 * @return None
**/
void SAADC_IRQHandler() { 
    uint32_t *event_start_address = (uint32_t*)0x40007100;
    if(*event_start_address) {
        *event_start_address = 0;
        conv_enb = true;
        return;
    }

    uint32_t *event_end_address = (uint32_t*)0x40007104;
    if(*event_end_address) {
        *event_end_address = 0;
        conv_enb = false;
        visualizer_color_info();
        pix_color_set(r, g, b);
        adc_read_samples(input);
        return;
    }
}
