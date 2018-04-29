/*
 * FTM.h
 *
 *  Created on: 16/04/2018
 *      Author: Farid
 */

#ifndef FTM_H_
#define FTM_H_

#include "system.h" /* include peripheral declarations */
#include "FTM_config.h"
#include "PORT.h"

typedef void (*vfcn_callback)(void);

void FTM_QD_mode_Init(FTM_QuadDec_config_t config, vfcn_callback callback);
void FTM_PWM_mode_Init(FTM_PWM_config_t config);
void PWM_set_duty(PWM_channel, uint32_t value);


#endif /* FTM_H_ */
