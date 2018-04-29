/*
 * Steering.h
 *
 *  Created on: 25/04/2018
 *      Author: Farid
 */

#ifndef STEERING_STEERING_H_
#define STEERING_STEERING_H_

#include "dual_vnh5019_config.h"
#include "FTM.h"
#include "ADC.h"
#include "arm_math.h"

extern PWM_channel M1_PWM;
extern PWM_channel M2_PWM;

typedef enum{
	CW,
	CCW,
	Stop,
	Coast
}steer_direction;

void Steering_init(void);
float32_t steering_encoder_read_rev(void);
float32_t steering_encoder_read_deg(void);
void steering_manual_ctrl(void);
void steering_set_position(float32_t set_point);
void set_direction(steer_direction dir);


#endif /* STEERING_STEERING_H_ */
