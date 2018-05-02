/*
 * Break.h
 *
 *  Created on: 29/04/2018
 *      Author: Andres
 */

#ifndef BREAK_BREAK_H_
#define BREAK_BREAK_H_

#include "LPUART.h"

void Break_init(void);
void Break_cmds(int Mdata, int Rdata, char fertig);

#endif /* BREAK_BREAK_H_ */
