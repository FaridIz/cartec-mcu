/*
 * dual_vnh5019_config.h
 *
 *  Created on: 23/04/2018
 *      Author: Farid
 */

#ifndef MOTORDRIVER_DUAL_VNH5019_CONFIG_H_
#define MOTORDRIVER_DUAL_VNH5019_CONFIG_H_

#include "PORT_config.h"

PORT_config_t dummy = {	// Green LED, just for testing
	.port	= ePortE,
	.pin	= 22,
	.mux	= eMuxAsGPIO,
	.dir	= eOutput,
};

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

PORT_config_t M1_PWM = {
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

PORT_config_t M2_PWM = {
	.port	= ePortB,
	.pin	= 0,
	.mux	= eMux6,	// FTM4_CH6
	.dir	= eOutput,
};



#endif /* MOTORDRIVER_DUAL_VNH5019_CONFIG_H_ */
