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
#include <stdint.h>
#include <timer.h>
#include <adc.h>
#include <pix.h>
#include <printk.h>
// #include <string.h>


#define R_LIM   (85) /**< define R_LIM 85 */
#define G_LIM   (170) /**< define G_LIM 170 */
#define RGB_MAX (255) /**< define RGB_MAX 255 */
#define SCALE_FACT (4) /**< define SCALE_FACT 4 */

static int16_t input[FFT_SIZE];     // FFT_SIZE real values
static int16_t output[FFT_SIZE*2];  // FFT_SIZE complex (real,imag) values
static int16_t mags[FFT_SIZE];      // fft output magnitudes
static uint8_t r, g, b;


/**
 * @brief implement visualizer_init function
 * (1) call visualizer_init function using freq parameter
 * @param freq freq used for adc_timer_start
 * @return None
**/
void visualizer_init(int freq) {
  // maybe some things to initialize here...
  (void) freq;
  adc_timer_start(freq);
  return;
}

/* similar to previous visualizer_color_info, mod'd to use global variables */
/**
 * @brief implement visualizer_color_info function
 * (1) set visualizer color using input array
 * @return None
**/
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
 * @brief implement TIMER0_IRQHandler function
 * (1) everytime when the timer0 interrupt is called, issue one task and take one sample
 * (2) issue task clear to reset the timer
 * (3) disable the nvic interrupt to reset the timer
 * @return None
**/
void TIMER0_IRQHandler() {
    // collect next adc sample from adc
    // take one SAADC sample
    uint32_t base_address = 0x40007000;
    uint32_t *task_sample_address = (uint32_t *)(base_address + 0x004);
    *task_sample_address = 1;
    
    // reset timer
    // disable icpr interrupt
    uint32_t *task_clear = (uint32_t *)0x4000800C;
    *task_clear = 1;

    uint32_t *nvic_icpr = (uint32_t *)0XE000E280;
    *nvic_icpr = (1 << 8);


}

/**
 * @brief implement SAADC_IRQHandler function
 * (1) set nvic icpr register 
 * (2) call adc_read_samples in adc interrupt
 * (3) and convert input array to visualizer and set pixel color
 * @return None
**/
void SAADC_IRQHandler() {
    uint32_t *nvic_icpr = (uint32_t *)0XE000E280;
    *nvic_icpr = (1 << 7);

    adc_read_samples(input);
    visualizer_color_info();
    pix_color_set(r, g, b);

}

