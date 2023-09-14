#include <visualizer.h>
#include <unistd.h>
#include <rfft.h>
#include <printk.h>

#define R_LIM   (85)
#define G_LIM   (170)
#define RGB_MAX (255)
#define SCALE_FACT (4)

static int16_t input[FFT_SIZE];     // FFT_SIZE real values
static int16_t output[FFT_SIZE*2];  // FFT_SIZE complex (real,imag) values
static int16_t mags[FFT_SIZE];      // fft output magnitudes

static int ind = 0;

int visualizer_push_back(int16_t adc_data) {
    if (ind == FFT_SIZE) {
        return -1;
    }
    input[ind] = adc_data;
    ind++;
    for (int i = 0; i < 1000; i++);
    /* FOR BONUS: Busy looping is a horrible way to enforce a constant sampling 
       rate. What other mechanisms can you use on our board to make sure that
       a constant sampling rate is enforced?
    */
    return 0;
}

void visualizer_new_collect() {
    ind = 0;
    return;
}

int visualizer_data_full() {
    return ind == FFT_SIZE;
}

int visualizer_color_info(uint8_t *r, uint8_t *g, uint8_t *b) {
    uint32_t r_avg, g_avg, b_avg;
    int i;
    if (r == NULL || g == NULL || b == NULL) {
        return -1;
    }
    
    rfft(input, output);
    rfft_abs(output, mags, FFT_SIZE);
    
    r_avg = g_avg = b_avg = 0;
    for(i = 2; i < R_LIM; i++)
        r_avg += mags[i];
    for(i = R_LIM; i < G_LIM; i++)
        g_avg += mags[i];
    for(i = G_LIM; i < FFT_SIZE; i++)
        b_avg += mags[i];

    r_avg >>= SCALE_FACT;
    g_avg >>= SCALE_FACT;
    b_avg >>= SCALE_FACT;
    r_avg = r_avg > RGB_MAX ? RGB_MAX : r_avg;
    g_avg = g_avg > RGB_MAX ? RGB_MAX : g_avg;
    b_avg = b_avg > RGB_MAX ? RGB_MAX : b_avg;
    // printk("%d %d %d", r_avg, g_avg, b_avg);
    // printk("\n");
    
    *r = r_avg;
    *g = g_avg;
    *b = b_avg;
    return 0;
}
