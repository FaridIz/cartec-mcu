/*
 * CruiseControl.c
 *
 *  Created on: 14/05/2018
 *      Author: Farid
 */

#include "CruiseControl.h"

PORT_config_t ENA = { .port = ePortC, .pin = 29, .mux = eMux2,      .dir = eOutput };
PORT_config_t IN1 = { .port = ePortC, .pin = 30, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t IN2 = { .port = ePortC, .pin = 31, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t IN4 = { .port = ePortE, .pin = 20, .mux = eMuxAsGPIO, .dir = eOutput };


FTM_PWM_config_t ENA_PWM_config = {
		.FTM_config.FTM_instance 	 = FTM5,
		.FTM_config.ip_index 		 = PCC_FTM5_INDEX,
		.FTM_config.FTM_clock_source = SOSCDIV1,	/* 8 MHz SOSCDIV1_CLK */

		.preescaler = PS_1,			/* (8MHz)/1 = 8MHz */
		.channels	= 0b100,			/* Channel 2 */
		.mod		= 400			/* 20KHz PWM period */
};

PWM_channel ENA_PWM = { .FTM_instance = FTM5, .number = 2 };


void cruisecontrol_init(void){
//	ADC_init();	//12bit resolution
	GPIO_pinInit(IN1);
	GPIO_pinInit(IN2);
	GPIO_pinInit(IN4);
	PORT_init(ENA);
	FTM_PWM_mode_Init(ENA_PWM_config);
}


void cruisecontrol_release(void){
	GPIO_clearPin(IN4);
}

void cruisecontrol_dummy(void){
	if(GPIO_readPin(SW3))
		GPIO_setPin(IN4);
	else
		GPIO_clearPin(IN4);
	if(GPIO_readPin(SW4)){
		GPIO_clearPin(IN2);
		GPIO_setPin(IN1);
		PWM_set_duty(ENA_PWM, 400);
	}
	else{
		GPIO_clearPin(IN1);
		GPIO_clearPin(IN2);
		PWM_set_duty(ENA_PWM, 0);
	}

}
