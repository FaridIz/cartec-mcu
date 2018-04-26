/*
 * Steering.h
 *
 *  Created on: 25/04/2018
 *      Author: Farid
 */

#ifndef STEERING_STEERING_H_
#define STEERING_STEERING_H_

#include "dual_vnh5019_config.h"
#include "FTM_config.h"
#include "FTM.h"

void Steering_init(void);
double steering_encoder_read(void);


#endif /* STEERING_STEERING_H_ */
