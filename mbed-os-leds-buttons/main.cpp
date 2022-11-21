/*
 * Mbed OS Simple Digital/PWM/Interrupt example using on board and external components.
 * Copyright (c) 2022 dimitris@computer.org
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PinNames.h"
#include "PwmOut.h"
#include "mbed.h"

// Standardized LED and button names for DISCO-L4S5I
// LED1     PA_5   // LED1 [GREEN]
// LED2     PB_14  // LED2 [GREEN]
// LED3     PC_9   // LED3_WIFI LED4_BLE [YELLOW]
// BUTTON1  PC_13  // BUTTON_EXTI13 [B2]

// Blinking rate in milliseconds
#define BLINKING_RATE_MS  250


DigitalOut led1(LED1);
PwmOut led2(ARDUINO_UNO_D15);
InterruptIn user_button(BUTTON1);

static BufferedSerial serial_port(USBTX, USBRX, 9600);
volatile float led_Intensity = 0.5; /*roughly 50% */

static void user_button_handler(){
    led_Intensity = led_Intensity+0.1;
    if (led_Intensity>=1.0) {
        led_Intensity=0;
    }
}

// main() runs in its own thread in the OS
int main()
{
	led1 = 0;
	led2 = 0.0;
 	//Interrupt handlers
	user_button.rise(&user_button_handler);
    
    while (true) {
		//led2 = !led2;
        led1 = !led1;

        if (led2 == 0) {
            led2 = led_Intensity;
        }
        else {
            led2 = 0;
        }
    
        printf("PWM: Value: [%f] | Period: [%d]us PulseWidth[%d]us \n ",led2.read(),led2.read_period_us(),led2.read_pulsewitdth_us());
		//thread_sleep_for(BLINKING_RATE_MS); /* see: https://github.com/pyocd/pyOCD/issues/1147 */
		wait_us(BLINKING_RATE_MS*1000);
	}
}
