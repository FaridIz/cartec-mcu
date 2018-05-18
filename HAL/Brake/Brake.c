/*
 * brake.c
 *
 *  Created on: 29/04/2018
 *      Author: Andres
 */


#include <Brake.h>

LPUART_config_t brake_sys = {
		.pin_rx.port = ePortA,
		.pin_rx.pin  = 8,
		.pin_rx.mux	 = eMux2,

		.pin_tx.port = ePortA,
		.pin_tx.pin  = 9,
		.pin_tx.mux	 = eMux2,

		.LPUART_instance 		= LPUART2,
		.ip_index		 		= PCC_LPUART2_INDEX,
		.LPUART_clock_source	= SOSCDIV2,
		.clock_src_freq			= 8000000,
		.Baudrate				= 9600
};

void brake_init (void){
	LPUART_init(brake_sys);
	LPUART_transmit_string(brake_sys, "Y\n\r");     /* Load default values of all settings and gains */
	LPUART_transmit_string(brake_sys, "P0\n\r");     /* Send encoder position to zero */
	LPUART_transmit_string(brake_sys, "S0\n\r");     /* Send speed to zero */
}

void brake_cmds (int Mdata, int Rdata, char fertig){
	if (fertig == 0){
		char M[] = {0x30 + Mdata/100, 0x30 + (Mdata/10)%10, 0x30 + Mdata%10}; //separate the reference variable into each digit for char array
		char R[] = {0x30 + Rdata/1000, 0x30 + (Rdata/100)%10, 0x30 + (Rdata/10)%10, 0x30 + Rdata%10}; //separate the reference variable into each digit for char array

		LPUART_send(brake_sys, 'M');
		LPUART_send(brake_sys, M[0]);
		LPUART_send(brake_sys, M[1]);
		LPUART_send(brake_sys, M[2]);
		LPUART_transmit_string(brake_sys, "\n\r");
		LPUART_send(brake_sys, 'R');
		LPUART_send(brake_sys, R[0]);
		LPUART_send(brake_sys, R[1]);
		LPUART_send(brake_sys, R[2]);
		LPUART_send(brake_sys, R[3]);
		LPUART_transmit_string(brake_sys, "\n\r");
	}else{
		char R[] = {0x2D, 0x30 + Rdata/1000, 0x30 + (Rdata/100)%10, 0x30 + (Rdata/10)%10, 0x30 + Rdata%10};

		LPUART_send(brake_sys, 'R');
		LPUART_send(brake_sys, R[0]);
		LPUART_send(brake_sys, R[1]);
		LPUART_send(brake_sys, R[2]);
		LPUART_send(brake_sys, R[3]);
		LPUART_send(brake_sys, R[4]);
		LPUART_transmit_string(brake_sys, "\n\r");
	}
}

uint32_t bandera = 0;

void dummy_brake(void){
	if (GPIO_readPin(SW3)){
		if (bandera == 0){
			brake_cmds(200, 360*5, 0);
			bandera = 1;
		}
	}else{
		if (bandera == 1){
			brake_cmds(200, 360*5, 1);
			bandera = 0;
		}
	}
	//delay(20);
}
