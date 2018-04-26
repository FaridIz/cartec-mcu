/*
 * Steering.c
 *
 *  Created on: 25/04/2018
 *      Author: Farid
 */


#include "Steering.h"

FTM_QuadDec_config_t steering_encoder = {
		/* Encoder channel A */
		.port[0].port	= ePortA,
		.port[0].pin	= 1,
		.port[0].mux 	= eMux5,
		.port[0].dir	= eInput,
		/* Encoder channel B */
		.port[1].port	= ePortB,
		.port[1].pin	= 2,
		.port[1].mux 	= eMux4,
		.port[1].dir	= eInput,
		/* FTM module configuration */
		.FTM_config.FTM_instance 	 = FTM1,
		.FTM_config.ip_index 		 = PCC_FTM1_INDEX,
		.FTM_config.FTM_clock_source = SPLLDIV1_CLK,
		.Ovf_IRQn	= FTM1_Ovf_Reload_IRQn,
		.mod		= 4096,							/* Ticks per revolution */
		.quadmode 	= phaseA_phaseB
};

int32_t count = 0;
void count_revolutions(void);

void Steering_init(void){
	FTM_QD_mode_Init(steering_encoder, count_revolutions);
//	vh5019_channel_1_init();
}

void count_revolutions(void){
	if (steering_encoder.FTM_config.FTM_instance->QDCTRL & FTM_QDCTRL_TOFDIR_MASK) {
		count++;
	}
	else {
		count--;
	}
}

double steering_encoder_read(void){
	double temp = (double) FTM1->CNT;
	temp /= 4096;
	temp += (double) count;
	return temp;
}
