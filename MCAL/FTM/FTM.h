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
//void FTM_PWM_mode_Init(QuadDec_config_t config);
//void set_FTM_PWM_dutycycle(FTM_Type * base, uint16_t value);


//void FTM2_Ovf_Reload_IRQHandler (void);


#endif /* FTM_H_ */
