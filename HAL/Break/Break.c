/*
 * Break.c
 *
 *  Created on: 29/04/2018
 *      Author: Andres
 */


#include "Break.h"

void Motor_setup(char Mstring[], char Rstring[]){
    LPUART1_transmit_string("P0\n\r");
    LPUART1_transmit_string("S0\n\r");
    LPUART1_transmit_string(Mstring);
    LPUART1_transmit_string(Rstring);
}

