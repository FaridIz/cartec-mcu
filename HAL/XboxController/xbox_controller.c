/*
 * xbox_controller.c
 *
 *  Created on: 21/05/2018
 *      Author: Farid
 */

#include "xbox_controller.h"


LPUART_config_t xbox_com = {
		.pin_rx.port = ePortC,
		.pin_rx.pin  = 6,
		.pin_rx.mux	 = eMux2,

		.pin_tx.port = ePortC,
		.pin_tx.pin  = 7,
		.pin_tx.mux	 = eMux2,

		.LPUART_instance 		= LPUART1,
		.ip_index		 		= PCC_LPUART1_INDEX,
		.LPUART_clock_source	= SOSCDIV2,
		.clock_src_freq			= 8000000,
		.Baudrate				= 57600
};


void xbox_controller_init(void){
	LPUART_init(xbox_com);
}

void xbox_controller(float *steering_position, float *brake, float *throttle){
	if(LPUART_readable(xbox_com) == -1) return;

	uint8_t xx = LPUART_get_uint8(xbox_com);
	if(xx != 195){
//		LPUART_flush_rx(xbox_com);
		return;
	}

	LPUART_send(xbox_com, '@');
	float buffer[3] = {0};
	uint8_t i;
	for(i=0; i<3; i++){
		buffer[i] = (float) LPUART_get_uint8(xbox_com);
		if(i!=2) LPUART_send(xbox_com, 'q');
	}

	*steering_position = (buffer[0] / 255*46) -23;
	*brake = buffer[2];
	*throttle = buffer[1] / 255*100;
}
