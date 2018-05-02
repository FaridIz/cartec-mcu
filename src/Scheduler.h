/*
 * Scheduler.h
 *
 *  Created on: 01/05/2018
 *      Author: Farid
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "system.h"

typedef void (*vfcn_callback)(void);

typedef struct {
	vfcn_callback task_callback;
	uint32_t period_ticks;
	uint32_t start_tick;
}scheduler_task_config_t;


void scheduler_init(scheduler_task_config_t * scheduler_task_array, uint8_t number_of_tasks ,uint32_t step_ticks);	//Each tick is 25ns


#endif /* SCHEDULER_H_ */
