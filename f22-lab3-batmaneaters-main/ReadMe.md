# Lab3: Pardon the Interruption, can I take your Order?

## Team Members:
Ruobing Wang (ruobing2)
Xingzhi Wang (xingzhi2)

## Tasks Breakdown:
In this lab, we created a SysTick interrupt, enabled the RESET button, and set sampling for TIMER-based ADC.
1. For Systick interrupt, we define four state to show the result of systick

2. For RESET button, we use the reset button on the board to reset the whole system. When the system reset, it will reach a breakpoint, only when we type 'continue', the kernel will initialize the whole system again

3. For TIMER-based ADC, we use timer interrupt and adc interrupt to convert the adc sample to the pixel color. 

We create a user space for syscalls and have an app that supports:

1. Six lights:
   1. User can change the color by tying "red," "blue," "green," "yellow," "white," and "purple."
   2. In default, we have white value (as it is pure white, so every command user chooses to do a color change will be very obvious)
2. Adaptive brightness, measured by lux sensor.
3. Calibration.
   1. We have a command "cali". Once the user chooses to calibrate the maximum to the current brightness, he can use this command.
4. Off
   1. We support an "off" command to turn the Neopixel off
5. End
   1. We support an "end" command to turn the Neopixel off and return the value -1. (terminate the whole app)
   2. Additionally, you will see the status light will turn to red!



