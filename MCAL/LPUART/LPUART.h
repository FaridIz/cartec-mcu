/*
 * lpuart.h
 *
 *  Created on: Apr 6, 2016
 *      Author: B46911
 */

#ifndef LPUART_H_
#define LPUART_H_

#include "device_registers.h" /* include peripheral declarations */

#if (defined(MCU_S32K142) || defined(MCU_S32K144) || defined(MCU_S32K146) || defined(MCU_S32K148))

void LPUART_Init(uint8_t ip_index, LPUART_Type * LPUART_base);
void LPUART_Send(LPUART_Type * LPUART_base, uint8_t data);
void LPUART_transmit_string(LPUART_Type * LPUART_base, char data_string[]);

#endif

#endif /* LPUART_LPUART_H_ */
