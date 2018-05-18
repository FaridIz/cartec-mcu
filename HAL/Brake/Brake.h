/*
 * brake.h
 *
 *  Created on: 29/04/2018
 *      Author: Andres
 */

#ifndef BRAKE_BRAKE_H_
#define BRAKE_BRAKE_H_

#include "LPUART.h"

void brake_init(void);
void brake_cmds(int Mdata, int Rdata, char fertig);

#endif /* BRAKE_BRAKE_H_ */
