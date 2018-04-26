/*
 * dual_vnh5019_config.c
 *
 *  Created on: 25/04/2018
 *      Author: Farid
 */

#include "dual_vnh5019_config.h"

void vnh5019_channel_1_init(void){
	GPIO_pinInit(M1_EN);
	GPIO_pinInit(M1_INA);
	GPIO_pinInit(M1_INB);
	PORT_init(M1_PWM_port);
}

void vnh5019_channel_2_init(void){
	GPIO_pinInit(M2_EN);
	GPIO_pinInit(M2_INA);
	GPIO_pinInit(M2_INB);
	PORT_init(M2_PWM_port);
}

PORT_config_t M1_EN = {
	.port	= ePortD,
	.pin	= 15,
	.mux	= eMuxAsGPIO,
	.dir	= eOutput,
};

PORT_config_t M1_INA = {
	.port	= ePortC,
	.pin	= 11,
	.mux	= eMuxAsGPIO,
	.dir	= eOutput,
};

PORT_config_t M1_INB = {
	.port	= ePortD,
	.pin	= 17,
	.mux	= eMuxAsGPIO,
	.dir	= eOutput,
};

PORT_config_t M1_PWM_port = {
	.port	= ePortA,
	.pin	= 30,
	.mux	= eMux2,	// FTM5_CH5
	.dir	= eOutput,
};

PORT_config_t M2_EN = {
	.port	= ePortB,
	.pin	= 3,
	.mux	= eMuxAsGPIO,
	.dir	= eOutput,
};

PORT_config_t M2_INA = {
	.port	= ePortD,
	.pin	= 14,
	.mux	= eMuxAsGPIO,
	.dir	= eOutput,
};

PORT_config_t M2_INB = {
	.port	= ePortA,
	.pin	= 31,
	.mux	= eMuxAsGPIO,
	.dir	= eOutput,
};

PORT_config_t M2_PWM_port = {
	.port	= ePortB,
	.pin	= 0,
	.mux	= eMux6,	// FTM4_CH6
	.dir	= eOutput,
};

