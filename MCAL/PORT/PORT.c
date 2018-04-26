/*
 * PORT.c
 *
 *  Created on: 23/04/2018
 *      Author: Farid
 */

#include "PORT.h"

void PORT_init(PORT_config_t config){
	/* Get proper PORT pointer */
	PORT_Type *port_ptrs[] = PORT_BASE_PTRS;
	/* Enable clock for port*/
	PCC->PCCn[PCC_PORTA_INDEX + (uint8_t)config.port] |= PCC_PCCn_CGC_MASK;
	/* Clear MUX field */
	port_ptrs[config.port]->PCR[config.pin] &= ~PORT_PCR_MUX_MASK;
	/* Write desire MUX value */
	port_ptrs[config.port]->PCR[config.pin] |= PORT_PCR_MUX(config.mux);
}
