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

		.preescaler = PS_1,			/* (8MHz)/1 = 8MHz */
		.channels	= 0b100000,		/* Channel 5 */
		.mod		= 400			/* 20KHz PWM period */
};

FTM_PWM_config_t channel_2_PWM = {
		.FTM_config.FTM_instance 	 = FTM4,
		.FTM_config.ip_index 		 = PCC_FTM4_INDEX,
		.FTM_config.FTM_clock_source = SOSCDIV1,	/* 8 MHz SOSCDIV1_CLK */

		.preescaler = PS_1,			/* (8MHz)/1 = 8MHz */
		.channels	= 0b1000000,	/* Channel 6 */
		.mod		= 400			/* 20KHz PWM period */
};

PWM_channel M1_PWM = {
		.FTM_instance = FTM5,
		.number	  = 5
};

PWM_channel M2_PWM = {
		.FTM_instance = FTM4,
		.number	  = 6
};

arm_pid_instance_f32 steering_pid = {
	.Kp = 0.125, 	//0.125
	.Ki = 0.0522, 	//0.0522,
	.Kd = 0.00654 	//0.00654,

};

#define pot_vs_pwmduty_relation  (2000/channel_1_PWM.mod)


/* =================================================================================== */

int32_t count = 0;
uint8_t pid_reset_flag = 0x00;
void count_revolutions(void);
void set_direction(steer_direction dir);
uint32_t potentiometer_position(void);

/* =================================================================================== */

void Steering_init(void){
	vnh5019_channel_2_init();
	ADC_init();	//12bit resolution
	FTM_QD_mode_Init(steering_encoder, count_revolutions);
	FTM_PWM_mode_Init(channel_2_PWM);
	arm_pid_init_f32(&steering_pid, 1);
}

float32_t steering_encoder_read_rev(void){
	float32_t temp = (float32_t) steering_encoder.FTM_config.FTM_instance->CNT;
	temp /= (float32_t) steering_encoder.mod;
	temp += (float32_t) count;
	return temp;
}

float32_t steering_encoder_read_deg(void){
	float32_t temp = (float32_t) steering_encoder.FTM_config.FTM_instance->CNT;
	temp /= (float32_t) steering_encoder.mod;
	temp += (float32_t) count;
	return (temp * 360);
}

/* =================================================================================== */

void steering_manual_ctrl(void){
	uint16_t value = potentiometer_position();
	uint16_t pwm_duty = 0;

	if(value <= 2000){
		pwm_duty = (2000 - value) / pot_vs_pwmduty_relation; //e.g. (0-2000)/(-5) = 400, (2000-2000)/(-5) = 0
		set_direction(CW);
	}
	else if((2000 < value) && (value < 3000)){
		GPIO_clearPin(M1_INA);
		GPIO_clearPin(M1_INB);
	}
	else if(value >= 3000){
		pwm_duty = (value - 3000)/pot_vs_pwmduty_relation;
		set_direction(CCW);
	}
	PWM_set_duty(M2_PWM, pwm_duty);
}

void steering_set_position(float32_t set_point){
	float32_t err;
	float32_t out;

	err = set_point - steering_encoder_read_deg();

	if( ((-PID_RESET_THRESHOLD < err) && (err < PID_RESET_THRESHOLD)) && ~pid_reset_flag ){
		arm_pid_reset_f32(&steering_pid);
		pid_reset_flag = 0xFF;
	}
	else if( ((err < -PID_RESET_THRESHOLD) || (PID_RESET_THRESHOLD < err)) && pid_reset_flag ){
		pid_reset_flag = 0x00;
	}

	out = arm_pid_f32(&steering_pid, err);

	if(out < 0){
		set_direction(CW);
		out *= -1;
	}
	else{
		set_direction(CCW);
	}
	if(out > 400)
		out = 400;

	PWM_set_duty(M2_PWM, out);
}


/* =================================================================================== */

void count_revolutions(void){
	if (steering_encoder.FTM_config.FTM_instance->QDCTRL & FTM_QDCTRL_TOFDIR_MASK) {
		count++;
	}
	else {
		count--;
	}
}

void set_direction(steer_direction dir){
	if(dir == CW){
		GPIO_setPin(M2_EN);
		GPIO_setPin(M2_INA);
		GPIO_clearPin(M2_INB);
	}
	else if (dir == CCW){
		GPIO_setPin(M2_EN);
		GPIO_clearPin(M2_INA);
		GPIO_setPin(M2_INB);
	}
	else if (dir == Stop){
		GPIO_clearPin(M2_INA);
		GPIO_clearPin(M2_INB);
	}
	else if (dir == Coast){
		GPIO_clearPin(M2_EN);
	}
}

uint32_t potentiometer_position(void){
	convertAdcChan(0b101100);		/* Convert Channel AD28 to pot on EVB */
	while(adc_complete()==0){}      /* Wait for conversion complete flag */
	return read_adc_chx();			/* Get channel's conversion results in mv (0-5000) */
}

