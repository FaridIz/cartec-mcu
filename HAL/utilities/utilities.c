/*
 * utilities.c
 *
 *  Created on: 14/05/2018
 *      Author: Farid
 */

#include "utilities.h"


PORT_config_t LED_RED   = { .port = ePortE, .pin = 21, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t LED_GREEN = { .port = ePortE, .pin = 22, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t LED_BLUE  = { .port = ePortE, .pin = 23, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t SW3  = { .port = ePortC, .pin = 12, .mux = eMuxAsGPIO, .dir = eInput, .attrib.filter = ePassiveFilterEnabled};
PORT_config_t SW4  = { .port = ePortC, .pin = 13, .mux = eMuxAsGPIO, .dir = eInput, .attrib.filter = ePassiveFilterEnabled };

void utilities_init(void){
	GPIO_pinInit(LED_RED);
	GPIO_pinInit(LED_GREEN);
	GPIO_pinInit(LED_BLUE);
	GPIO_pinInit(SW3);
	GPIO_pinInit(SW4);
	ADC_init();	//12bit resolution
}

uint32_t utility_potentiometer_position(void){
	convertAdcChan(0b101100);		/* Convert Channel AD28 to pot on EVB */
	while(adc_complete()==0){}      /* Wait for conversion complete flag */
	return read_adc_chx();			/* Get channel's conversion results in mv (0-5000) */
}
