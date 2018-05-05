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


void obd2_(){

}
