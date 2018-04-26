/*
 * FTM.c
 *
 *  Created on: 16/04/2018
 *      Author: Farid
 */

#include "FTM.h"


static vfcn_callback ftm_callback = 0;

/* Initialize FTM Module in Quadrature Decoder Mode */
void FTM_QD_mode_Init(FTM_QuadDec_config_t config, vfcn_callback callback)
{
	/* Port initialization configuration */
	PORT_init(config.port[0]);
	PORT_init(config.port[1]);

	FTM_Type * FTM_base = config.FTM_config.FTM_instance;

	/* Save callback */
	ftm_callback = callback;

	/* Select and enable, clock for FTM */
	PCC->PCCn[config.FTM_config.ip_index] = PCC_PCCn_PCS(config.FTM_config.FTM_clock_source) //6
										  | PCC_PCCn_CGC_MASK;
	/* Enable Overflow interrupt */
	S32_NVIC->ICPR[config.Ovf_IRQn / 32] |= (1 << (config.Ovf_IRQn % 32));
	S32_NVIC->ISER[config.Ovf_IRQn / 32] |= (1 << (config.Ovf_IRQn % 32));
	/* Decoding configuration */
	FTM_base->MOD 	 = FTM_MOD_MOD(config.mod - 1);				/* ie. Ticks per revolution */
	FTM_base->CNTIN  = FTM_CNTIN_INIT(0);
	FTM_base->QDCTRL = FTM_QDCTRL_QUADMODE(config.quadmode)		/* QUADMODE = 0  Phase A and B encoding mode */
					 | FTM_QDCTRL_QUADEN_MASK;					/* Enable Quadrature Decoder Mode*/
																/* No Filters */
																/* Normal Polarity */
	/* Reset Counter (Update CNT with CNTIN) */
	FTM_base->CNT = 0;
	/* Select counter clock source to be "FTM input clock" and enable Overflow Interrupt*/
	FTM_base->SC = FTM_SC_CLKS(1)
			 	 | FTM_SC_TOIE_MASK;
}


void FTM_PWM_mode_Init(FTM_PWM_config_t config)
{
	FTM_Type * FTM_base = config.FTM_config.FTM_instance;

	PCC->PCCn[config.FTM_config.ip_index] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config */
	PCC->PCCn[config.FTM_config.ip_index] |= PCC_PCCn_PCS(0b001)	/* Clock Source=1 */
	                    				  |  PCC_PCCn_CGC_MASK;  	/* Enable clock for FTM regs */

	/* Write protect to registers disabled (default) */
	FTM_base->MODE |= FTM_MODE_WPDIS_MASK;

	/* Preescaler selection */
	FTM_base->SC |= FTM_SC_PS(config.preescaler);	/* TOIE (Timer Overflow Interrupt Ena) = 0 (default) */
		                            				/* CPWMS (Center aligned PWM Select) = 0 (default, up count) */
		                            				/* CLKS (Clock source) = 0 (default, no clock; FTM disabled) */

	/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0  */
	FTM_base->COMBINE = 0x00000000;
	/* Polarity for all channels is active high (default) */
	FTM_base->POL = 0x00000000;

	/* Enable PWM channels */
	FTM_base->SC |= config.channels << FTM_SC_PWMEN0_SHIFT;

	/* PWM configuration */
	FTM_base->MOD = config.mod - 1 ;					/* FTM counter final value (used for PWM mode) */
		                            					/* FTM Period = MOD-CNTIN+0x0001 ctr clks */
	int i;
	for(i=0; i<8; i++){
		if( config.channels & (1<<i) ){
			FTM_base->CONTROLS[i].CnSC = 0x00000028;	/* FTM channel i: edge-aligned PWM, low true pulses */
				                                      	/* CHIE (Chan Interrupt Ena) = 0 (default) */
				                                      	/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM */
				                                      	/* ELSB:ELSA (chan Edge/Level Select)=0b10, low true */
			FTM_base->CONTROLS[i].CnV =  0;				/* FTM channel i compare value (~0% duty cycle) */
		}
	}

	/* Start FTM counter with clk source = external clock */
	FTM_base->SC |= FTM_SC_CLKS(3);
}


void PWM_set_duty(PWM_channel channel, uint32_t value){
	channel.FTM_instance->CONTROLS[channel.number].CnV =  value;
}


/* Timer overflow interrupt routine indicating one mechanical revolution */
void FTM1_Ovf_Reload_IRQHandler (void)	//FTM2_IRQHandler
{
	FTM1->SC &= ~FTM_SC_TOF_MASK;
	if (ftm_callback != 0){
		ftm_callback();
	}
}


