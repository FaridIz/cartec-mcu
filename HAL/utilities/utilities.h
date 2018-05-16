/*
 * utilities.h
 *
 *  Created on: 14/05/2018
 *      Author: Farid
 */

#ifndef UTILITIES_UTILITIES_H_
#define UTILITIES_UTILITIES_H_

#include "GPIO.h"
#include "ADC.h"
#define BENCH_TOOLS	/* Comment line if bench tools are not to be used */

extern PORT_config_t LED_RED;
extern PORT_config_t LED_GREEN;
extern PORT_config_t LED_BLUE;
extern PORT_config_t SW3;
extern PORT_config_t SW4;

void utilities_init(void);
uint32_t utility_potentiometer_position(void);



#ifdef BENCH_TOOLS		/* Miscellany functions for bench testing */

void delay(uint32_t ms);	/* Polling delay, maximum 1000ms */
void stopwatch(void);

#endif

#endif /* UTILITIES_UTILITIES_H_ */
