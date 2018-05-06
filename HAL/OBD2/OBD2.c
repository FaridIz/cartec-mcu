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

void hex2ascii(uint8_t byte, char *L, char *H);


void obd2_init(void){
	LPUART_init(OBD2);
	uint8_t i, n;
	uint8_t buffer[BUFFER_SIZE] = {0};	// When using a value different from 0: {[0 ... (length-1)] = 1};

	LPUART_flush_rx(OBD2); 				// When powering ELM327 on, it throws a message

	/* Configure ELM327 */
	char *initcmd[4] = {"ATZ\r", "ATE0\r", "ATL0\r", "ATH0\r"};	/* Reset, Echo off, Linefeed off, Headers off */

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

void obd2_readPID(uint8_t PID, int32_t *result){
	uint8_t buffer[40] = {0};
	char L, H;
	hex2ascii(PID, &L, &H);
	/* Send message */
	LPUART_transmit_string(OBD2, "01");
	LPUART_send(OBD2, H);
	LPUART_send(OBD2, L);
	LPUART_transmit_string(OBD2, " 1\r");	// To expect 1 single answer and avoid polling
	int8_t i=-1;
	do{
		i++;
		buffer[i] = LPUART_get_uint8(OBD2);
	}while(buffer[i] != '>' && buffer[i] != '?' );

	/* Extract PID data from string*/
	uint8_t end;
	uint32_t temp = 0;
	if(buffer[i] == '>' && buffer[i-3] == ' '){ // Verify that message is not an "UNABLE TO CONNECT" or so.
		end = i - 3;
		for(i=0; i < end; i++){

		}
	}

}

/* ELM327 message examples, when requesting PID, under ATE0, ATL0 and ATH0 configurations:
 * SEARCHING...\rUNABLE TO CONNECT\r\r>
 * SEARCHING...\r41 11 27 \r\r>
 * 41 11 28 \r\r>
 * 41 0c 00 00 \r\r>
 * */

void hex2ascii(uint8_t byte, char *L, char *H){
	*L = (byte & 0x0F);
	*H = (byte & 0xF0) >> 4;

	if(*L < 0x0A) *L += 0x30;
	else *L += 0x41;

	if(*H < 0x0A) *H += 0x30;
	else *H += 0x41;
}

void ascii2hex(){

}

