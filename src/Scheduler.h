/*
 * Scheduler.h
 *
 *  Created on: 01/05/2018
 *      Author: Farid
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "system.h"

#define scheduler_ticks 40000	//40000 ticks = 1ms

typedef void (*vfcn_callback)(void);


void scheduler_init(vfcn_callback task1, vfcn_callback task2, vfcn_callback task3);



#endif /* SCHEDULER_H_ */
