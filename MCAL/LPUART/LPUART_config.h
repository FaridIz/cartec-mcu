/*
 * LPUART_config.h
 *
 *  Created on: 30/04/2018
 *      Author: Farid
 */

#ifndef LPUART_CONFIG_H_
#define LPUART_CONFIG_H_

#include "S32K148.h"
#include "PORT_config.h"

typedef enum{
	SOSCDIV2 = 1,
	SIRCDIV2 = 2,
	FIRCDIV2 = 3,
	SPLLDIV2 = 6
}LPUART_clock_source_t;

typedef struct {
	PORT_config_t pin_tx;
	PORT_config_t pin_rx;
	LPUART_Type * LPUART_instance;
	uint8_t ip_index;
	LPUART_clock_source_t LPUART_clock_source;
	uint32_t clock_src_freq;	// No warranty that Baudrate is properly configured with a clock_src_freq other than 80MHz.
	uint16_t Baudrate;
}LPUART_config_t;


#endif /* LPUART_CONFIG_H_ */
