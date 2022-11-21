/*
 * Mbed OS SPI interface to an 8-Stage Shift and Store Bus Register.
 * See mbed-os-spi-leds.drawio functional diagram.
 * Copyright (c) 2022 dimitris@computer.org
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PinNames.h"
#include "PwmOut.h"
#include "Ticker.h"
#include "mbed.h"
#include <cstdio>
#include <memory>

#define SLEEP_RATE_MS     (200)
#define SWITCH_PATTERN_MS (5000)

BufferedSerial serial_port(USBTX, USBRX, 9600);
DigitalOut CS(ARDUINO_UNO_D10);
SPI ser_port(ARDUINO_UNO_D11, ARDUINO_UNO_D12, ARDUINO_UNO_D13);
InterruptIn user_button(BUTTON1);

static unsigned long elapsedtime = 0;
static char pattern = 2;
static const char patterntable[] = { 0x00,0x00,
                                     0x01,0x00,
                                     0x02,0x00,
                                     0x04,0x00,
                                     0x08,0x00,
                                     0x00,0xFF,
                                     0x55,0xAA,
                                     0xFF,0xFF};
static char data[] = {0x55,0xAA};
static bool advancepattern = false;

static void user_button_handler(){
	advancepattern = (elapsedtime < SWITCH_PATTERN_MS)?true:false;
}

int main()
{
	char txdata = 0x00;
	char rxdata = 0xFF;
	bool switchpattern = false;

	/*SPI setup, 8bits Mode0 1MHz*/
	ser_port.format(8,0);
	ser_port.frequency(1000000);
	CS=0;
	user_button.rise(&user_button_handler);

	/*forever*/
	while (1) {

		if ( advancepattern || (elapsedtime >= SWITCH_PATTERN_MS) ) {
			elapsedtime = 0;
			/*load another pattern*/
			std::memcpy(data,&patterntable[pattern],2);
			if (advancepattern) {
				printf("USER BUTTON pressed - pattern[%d], pattern data [0x%x 0x%x] \n",pattern/2, data[0],data[1]);
				advancepattern=false;
			}
			else {
				printf("pattern[%d], pattern data [0x%x 0x%x] \n",pattern/2, data[0],data[1]);
			}
			pattern = (pattern+2)%sizeof(patterntable);
			switchpattern = false;
		}

		txdata= data[switchpattern];
		CS=0;
		ser_port.write(&txdata, 1, &rxdata, 1);
		CS=1;

		//thread_sleep_for(SLEEP_RATE_MS); /* see: https://github.com/pyocd/pyOCD/issues/1147 */
		wait_us(SLEEP_RATE_MS*1000);

		switchpattern = !switchpattern;
		elapsedtime += SLEEP_RATE_MS;
	}
}
