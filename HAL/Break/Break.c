/*
 * Break.c
 *
 *  Created on: 29/04/2018
 *      Author: Andres
 */


#include "Break.h"

void Motor_setup (int Mdata, int Rdata, char fertig){
	LPUART1_transmit_string("P0\n\r");     /* Transmit char string */
	LPUART1_transmit_string("S0\n\r");     /* Transmit char string */

	if (fertig == 0){
		char M[] = {0x30 + Mdata/100, 0x30 + (Mdata/10)%10, 0x30 + Mdata%10}; //separate the reference variable into each digit for char array
		char R[] = {0x30 + Rdata/1000, 0x30 + (Rdata/100)%10,
					0x30 + (Rdata/10)%10, 0x30 + Rdata%10}; //separate the reference variable into each digit for char array

		LPUART1_transmit_char('M'); //set maximum speed of motor
		LPUART1_transmit_char(M[0]);
		LPUART1_transmit_char(M[1]);
		LPUART1_transmit_char(M[2]);
		LPUART1_transmit_string("\n\r");
		LPUART1_transmit_char('R');
		LPUART1_transmit_char(R[0]);
		LPUART1_transmit_char(R[1]);
		LPUART1_transmit_char(R[2]);
		LPUART1_transmit_char(R[3]);
		LPUART1_transmit_string("\n\r");
	}else{
		char R[] = {0x2D, 0x30 + Rdata/1000, 0x30 + (Rdata/100)%10,
				0x30 + (Rdata/10)%10, 0x30 + Rdata%10};
		LPUART1_transmit_char('R');
		LPUART1_transmit_char(R[0]);
		LPUART1_transmit_char(R[1]);
		LPUART1_transmit_char(R[2]);
		LPUART1_transmit_char(R[3]);
		LPUART1_transmit_char(R[4]);
		LPUART1_transmit_string("\n\r");
	}
}
