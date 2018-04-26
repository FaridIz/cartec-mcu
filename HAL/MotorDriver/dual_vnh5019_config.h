/*
 * dual_vnh5019_config.h
 *
 *  Created on: 23/04/2018
 *      Author: Farid
 */

#ifndef MOTORDRIVER_DUAL_VNH5019_CONFIG_H_
#define MOTORDRIVER_DUAL_VNH5019_CONFIG_H_

#include "PORT_config.h"
#include "GPIO.h"
#include "FTM.h"

extern PORT_config_t dummy;
extern PORT_config_t M1_EN;
extern PORT_config_t M1_INA;
extern PORT_config_t M1_INB;
extern PORT_config_t M1_PWM;
extern PORT_config_t M2_EN;
extern PORT_config_t M2_INA;
extern PORT_config_t M2_INB;
extern PORT_config_t M2_PWM;

void vnh5019_channel_1_init(void);
void vnh5019_channel_2_init(void);


#endif /* MOTORDRIVER_DUAL_VNH5019_CONFIG_H_ */
