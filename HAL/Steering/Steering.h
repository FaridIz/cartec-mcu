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
#include "ADC.h"

extern PWM_channel M1_PWM;
extern PWM_channel M2_PWM;

void Steering_init(void);
double steering_encoder_read(void);

void steering_manual_ctrl(void);

#endif /* STEERING_STEERING_H_ */
