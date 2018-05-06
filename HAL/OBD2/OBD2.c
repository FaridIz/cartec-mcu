/*
 * OBD2.c
 *
 *  Created on: 02/05/2018
 *      Author: Farid
 */

#include "OBD2.h"

LPUART_config_t OBD2 = {
		.pin_rx.port = ePortA,
		.pin_rx.pin  = 27,
		.pin_rx.mux	 = eMux4,

		.pin_tx.port = ePortA,
		.pin_tx.pin  = 28,
		.pin_tx.mux	 = eMux4,

		.LPUART_instance 		= LPUART0,
		.ip_index		 		= PCC_LPUART0_INDEX,
		.LPUART_clock_source	= SOSCDIV2,
		.clock_src_freq			= 8000000,
		.Baudrate				= 38400
};


void obd2_init(void){
	LPUART_init(OBD2);
	uint8_t i, n;
	uint8_t buffer[BUFFER_SIZE] = {0};	// When using a value different from 0: {[0 ... (length-1)] = 1};

	LPUART_flush_rx(OBD2); 				// When powering ELM327 on, it throws a message

	/* Configure ELM327 */
	uint8_t *initcmd[4] = {"ATZ\r", "ATE0\r", "ATL0\r", "ATH0\r"};	/* Reset, Echo off, Linefeed off, Headers off */

	for(i =0; i< 4; i++){
		LPUART_transmit_string(OBD2, initcmd[i]);
		for(n=0; n < BUFFER_SIZE; n++){
			buffer[n] = LPUART_get_uint8(OBD2);
			if(buffer[n] == '?')
				i--;					// Try again
			else if(buffer[n] == '>')
				break;
		}
	}

	/* Select protocol */
//	LPUART_transmit_string(OBD2, "ATSP0\r");	// Automatic (default)
//	LPUART_flush_rx(OBD2);

}
