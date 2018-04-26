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





//void FTM_PWM_mode_Init(QuadDec_config_t QuadDec_config)
//{
//	PCC->PCCn[QuadDec_config.FTM_config.ip_index] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config */
//	PCC->PCCn[QuadDec_config.FTM_config.ip_index] |= PCC_PCCn_PCS(0b001)	/* Clock Src=1, 8 MHz SOSCDIV1_CLK */
//	                    						  |  PCC_PCCn_CGC_MASK;   /* Enable clock for FTM regs */
//
//	/* Write protect to registers disabled (default) */
//	base->MODE |= FTM_MODE_WPDIS_MASK;
//
//
//	base->SC = 0x00030003;	 	/* Enable PWM channel 0 output*/
//	                            /* Enable PWM channel 1 output*/
//	                            /* TOIE (Timer Overflow Interrupt Ena) = 0 (default) */
//	                            /* CPWMS (Center aligned PWM Select) = 0 (default, up count) */
//	                            /* CLKS (Clock source) = 0 (default, no clock; FTM disabled) */
//	                            /* PS (Prescaler factor) = 3. Prescaler = 8 */
//	base->COMBINE = 0x00000000;	/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0  */
//	base->POL = 0x00000000;		/* Polarity for all channels is active high (default) */
//
//	/* PWM configuration */
//	base->MOD = 100 -1 ;					/* FTM1 counter final value (used for PWM mode) */
//		                            		/* FTM1 Period = MOD-CNTIN+0x0001 ~= 100 ctr clks  */
//		                            		/* 8MHz /128 = 1MHz ->  ticks -> 10KHz */
//	base->CONTROLS[1].CnSC = 0x00000028;  	/* FTM0 ch1: edge-aligned PWM, low true pulses */
//	                                      	/* CHIE (Chan Interrupt Ena) = 0 (default) */
//	                                      	/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM*/
//	                                      	/* ELSB:ELSA (chan Edge/Level Select)=0b10, low true */
//	base->CONTROLS[1].CnV =  75;       	/* FTM0 ch1 compare value (~75% duty cycle) */ // 100*0.75
//
//	base->SC |= FTM_SC_CLKS(3);	/* Start FTM0 counter with clk source = external clock (SOSCDIV1_CLK)*/
//}


void set_FTM_PWM_dutycycle(FTM_Type * base, uint16_t value){
	base->CONTROLS[1].CnV =  value;
}


/* Timer overflow interrupt routine indicating one mechanical revolution */
void FTM1_Ovf_Reload_IRQHandler (void)	//FTM2_IRQHandler
{
	FTM1->SC &= ~FTM_SC_TOF_MASK;
//	PTE->PTOR|=1<<23;
	if (ftm_callback != 0)
	{
		ftm_callback();
	}
}


