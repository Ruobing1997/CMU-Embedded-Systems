#include <timer.h>
#include <unistd.h>
#include <printk.h>
#include <stdint.h>
#include <arm.h>
#include <adc.h>

/**
 * @brief global value for counting tick
**/
uint32_t tick = 0;

/**
 * @brief define CSR register for later use
**/
uint32_t* CSR_reg = (uint32_t *)0xE000E010;

/**
 * @brief define RVR register for later use
**/
uint32_t* RVR_reg = (uint32_t *)0xE000E014;

/**
 * @brief define counterflag mask for getting counter flag value
**/
int CSR_COUNTFLAG_MSK = (1 << 16);

/**
 * @brief implement systick start function
 * (1) set CSR register value
 * (2) set RVR register value as 0.25s, 64000 is 1ms
 * (3) set NVIC register to enable interrupt
 * @return None
**/
void systick_start() {
    *CSR_reg = 0x7;
    // set the RVR register to 1 ms
    *RVR_reg = 64000 * 250;
    // set NVIC register
    // set 32 bit
    uint32_t* nvic_iser = (uint32_t *)0xE000E100;
    *nvic_iser |= (1 << 31);
    return;
}

/**
 * @brief implement systick handler 
 * (1) define tick for different state
 * (2) for four states print different charactor
 * @return None
**/
void SysTick_Handler() {
    // configure serial output for one-second timer
    // three state for serial output: -, /, |, '\'
    tick += 1;
    if (tick % 4 == 0) {
        printk("\b");
        printk("-");
    } 
    else if (tick % 4 == 1) {
        printk("\b");
        printk("\\");
    } else if (tick % 4 == 2) {
        printk("\b");
        printk("|");
    } else {
        printk("\b");
        printk("/");
    }
    return;
}

/**
 * @brief implement systick start function
 * (1) set CSR and RVR register
 * (2) wait for ms and then reset the CSR register
 * @param ms ms is the time used for delay function
 * @return None
**/
void systick_delay(uint32_t ms) {
    (void) ms;
    uint32_t ticks = 0; 
    *CSR_reg = 0x7; 
    *RVR_reg = 64000; 
    while(ticks < ms) {
        while(!(*CSR_reg & CSR_COUNTFLAG_MSK)); 
        ticks++; 
    }
    *CSR_reg = 0;
    return;
}

/**
 * @brief implement systick start function
 * (1) configure timer instance 0
 * (2) configure prescaler value and enable interrupt
 * (3) configure bitmode to 3
 * (4) set CC[0] register value 
 * (5) NVIC register, enable TIMER0_interrupt
 * (6) start the timer instance
 * @param freq freq is used for configuring timer interrupt
 * @return None
**/
void adc_timer_start(int freq) {
    (void) freq;
    // freq, adc sample rate
    // user timer0 instance
    uint32_t timer0_address = 0x40008000;

    // set prescaler value to 0
    uint32_t *prescalar_address = (uint32_t *)(timer0_address + 0x510);
    *prescalar_address = 0;

    // enable interrupt
    uint32_t* intenset_address = (uint32_t *)(timer0_address + 0x304);
    // set 4 compare registers to enable interrupts
    // enable compare register 0
    *intenset_address |= (1 << 16);


    // configure bitmode, set bitmode to maximum
    uint32_t* bitmode = (uint32_t *)(timer0_address + 0x508);
    *bitmode = 3;

    // CC[0] count
    uint32_t cc_compare = 16000000 / freq;
    uint32_t* cc0_address = (uint32_t *)(timer0_address + 0x540);
    *cc0_address = cc_compare;

    // configure NVIC register
    uint32_t* nvic_iser = (uint32_t *)0xE000E100;
    // enable the TIMER0_IRQHandler
    *nvic_iser |= (1 << 8);

    // start task
    uint32_t *task_address = (uint32_t *)timer0_address;
    *task_address = 1;

    return;
}


