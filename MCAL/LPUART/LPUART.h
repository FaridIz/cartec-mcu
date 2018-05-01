/*
 * lpuart.h
 *
 *  Created on: Apr 6, 2016
 *      Author: B46911
 */

#ifndef LPUART_H_
#define LPUART_H_

#include "system.h" /* include peripheral declarations */
#include "LPUART_config.h"
#include "PORT.h"

#define DEFAULT_OSR 16

void LPUART_init(LPUART_config_t config);

void LPUART_send(LPUART_config_t config, uint8_t data);
int16_t LPUART_receive(LPUART_config_t config);
uint8_t LPUART_get_uint8(LPUART_config_t config);
int8_t LPUART_readable(LPUART_config_t config);		/* Determine if there is a character available to read.  */
void LPUART_transmit_string(LPUART_config_t config, uint8_t data_string[]);

#endif /* LPUART_LPUART_H_ */
