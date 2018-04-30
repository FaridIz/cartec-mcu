/*
 * Break.c
 *
 *  Created on: 29/04/2018
 *      Author: Andres
 */


#include "Break.h"

void Motor_init (void){
	LPUART_transmit_string(LPUART2, "Y\n\r");     /* Load default values of all settings and gains */
	LPUART_transmit_string(LPUART2, "P0\n\r");     /* Set position to zero */
	LPUART_transmit_string(LPUART2, "S0\n\r");     /* Set speed to zero */
}

void Motor_cmds (int Mdata, int Rdata, char fertig){

	if (Rdata >= 700){
		Rdata = 700;
	}
	if (fertig == 0){
		char M[] = {0x30 + Mdata/100, 0x30 + (Mdata/10)%10, 0x30 + Mdata%10}; //separate the reference variable into each digit for char array
		char R[] = {0x30 + (Rdata/100)%10, 0x30 + (Rdata/10)%10, 0x30 + Rdata%10}; //separate the reference variable into each digit for char array

		LPUART_Send(LPUART2, 'M'); //set maximum speed of motor
		LPUART_Send(LPUART2, M[0]);
		LPUART_Send(LPUART2, M[1]);
		LPUART_Send(LPUART2, M[2]);
		LPUART_transmit_string(LPUART2, "\n\r");
		LPUART_Send(LPUART2, 'R');
		LPUART_Send(LPUART2, R[0]);
		LPUART_Send(LPUART2, R[1]);
		LPUART_Send(LPUART2, R[2]);
		LPUART_transmit_string(LPUART2, "\n\r");
	}else{
		char R[] = {0x2D, 0x30 + Rdata/1000, 0x30 + (Rdata/100)%10,
				0x30 + (Rdata/10)%10, 0x30 + Rdata%10};
		LPUART_Send(LPUART2, 'R');
		LPUART_Send(LPUART2, R[0]);
		LPUART_Send(LPUART2, R[1]);
		LPUART_Send(LPUART2, R[2]);
		LPUART_Send(LPUART2, R[3]);
		LPUART_Send(LPUART2, R[4]);
		LPUART_transmit_string(LPUART2, "\n\r");
	}
}
