/*
 * Steering.c
 *
 *  Created on: 25/04/2018
 *      Author: Farid
 */


#include "Steering.h"

/* =================================================================================== */

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
		.FTM_config.FTM_clock_source = SPLLDIV1,
		.Ovf_IRQn	= FTM1_Ovf_Reload_IRQn,
		.mod		= 4096,							/* Ticks per revolution */
		.quadmode 	= phaseA_phaseB
};

FTM_PWM_config_t channel_1_PWM = {
		.FTM_config.FTM_instance 	 = FTM5,
		.FTM_config.ip_index 		 = PCC_FTM5_INDEX,
		.FTM_config.FTM_clock_source = SOSCDIV1,	/* 8 MHz SOSCDIV1_CLK */

		.preescaler = PS_2,			/* (8MHz)/2 = 4MHz */
		.channels	= 0b100000,		/* Channel 5 */
		.mod		= 1000			/* 4KHz PWM period */
};

FTM_PWM_config_t channel_2_PWM = {
		.FTM_config.FTM_instance 	 = FTM4,
		.FTM_config.ip_index 		 = PCC_FTM4_INDEX,
		.FTM_config.FTM_clock_source = SOSCDIV1,	/* 8 MHz SOSCDIV1_CLK */

		.preescaler = PS_2,			/* (8MHz)/2 = 4MHz */
		.channels	= 0b1000000,	/* Channel 6 */
		.mod		= 1000			/* 4KHz PWM period */
};

PWM_channel M1_PWM = {
		.FTM_instance = FTM5,
		.number	  = 5
};

PWM_channel M2_PWM = {
		.FTM_instance = FTM4,
		.number	  = 6
};

/* =================================================================================== */

int32_t count = 0;
void count_revolutions(void);

/* =================================================================================== */

void Steering_init(void){
	/* Encoder initialization */
	FTM_QD_mode_Init(steering_encoder, count_revolutions);
	/* Motor driver initialization */
	vnh5019_channel_1_init();
	FTM_PWM_mode_Init(channel_1_PWM);
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
