/*
 * FTM_config.h
 *
 *  Created on: 25/04/2018
 *      Author: Farid
 */

#ifndef FTM_FTM_CONFIG_H_
#define FTM_FTM_CONFIG_H_

#include "system.h"
#include "PORT_config.h"

/* ================================================================== */
typedef enum{
	TCLK = 0,
	SOSCDIV1_CLK = 1,
	SIRCDIV1_CLK = 2,
	FIRCDIV1_CLK = 3,
	SPLLDIV1_CLK = 6
}FTM_clock_source_t;

typedef enum{
	phaseA_phaseB,
	count_and_direction
}FTM_QuadDec_QuadMode_t;

/* ================================================================== */
typedef struct {
	FTM_Type * FTM_instance;
	uint8_t ip_index;
	FTM_clock_source_t FTM_clock_source;
}FTM_config_t;

/* ================================================================== */
typedef struct {
	PORT_config_t port[2];
	FTM_config_t FTM_config;
	uint8_t Ovf_IRQn;
	uint16_t mod;
	FTM_QuadDec_QuadMode_t quadmode;
}FTM_QuadDec_config_t;



#endif /* FTM_FTM_CONFIG_H_ */
