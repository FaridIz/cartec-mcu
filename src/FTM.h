/*
 * FTM.h
 *
 *  Created on: 16/04/2018
 *      Author: Farid
 */

#ifndef FTM_H_
#define FTM_H_

#include "S32K148.h" /* include peripheral declarations */


typedef void (*vfcn_callback)(void);

void FTM_QD_mode_Init(uint8_t ip_index, uint8_t Ovf_IRQn, FTM_Type * base, uint16_t TicksPerCntOvf, vfcn_callback callback);
void FTM_PWM_mode_Init(uint8_t ip_index, FTM_Type * base);
void set_FTM_PWM_dutycycle(FTM_Type * base, uint16_t value);


//void FTM2_Ovf_Reload_IRQHandler (void);


#endif /* FTM_H_ */
