/*
 * FTM.c
 *
 *  Created on: 16/04/2018
 *      Author: Farid
 */

#include "FTM.h"

static vfcn_callback ftm_callback = 0;

/* Initialize FTM Module in Quadrature Decoder Mode */
void FTM_QD_mode_Init(uint8_t ip_index, uint8_t Ovf_IRQn, FTM_Type * base, uint16_t TicksPerCntOvf, vfcn_callback callback)
{
	/* Save callback */
	ftm_callback = callback;

	/* Select and enable, clock for FTM */
	PCC->PCCn[ip_index] = PCC_PCCn_PCS(6)
						| PCC_PCCn_CGC_MASK;

	/* Enable Overflow interrupt */
	S32_NVIC->ICPR[Ovf_IRQn / 32] |= (1 << (Ovf_IRQn % 32));
	S32_NVIC->ISER[FTM2_Ovf_Reload_IRQn / 32] |= (1 << (FTM2_Ovf_Reload_IRQn % 32));

	/* Decoding configuration */
	base->MOD = FTM_MOD_MOD(TicksPerCntOvf - 1);		/* ie. Ticks per revolution */
	base->CNTIN = FTM_CNTIN_INIT(0);
	base->QDCTRL= FTM_QDCTRL_QUADEN_MASK;		/* Enable Quadrature Decoder Mode*/
												/* No Filters */
												/* Normal Polarity */
												/* QUADMODE = 0  Phase A and B encoding mode */

	/* Reset Counter (Update CNT with CNTIN) */
	base->CNT = 0;

	/* Select counter clock source to be "FTM input clock" and enable Overflow Interrupt*/
	FTM2->SC = FTM_SC_CLKS(1)
			 | FTM_SC_TOIE_MASK;
}


/* Timer overflow interrupt routine indicating one mechanical revolution */
void FTM2_Ovf_Reload_IRQHandler (void)	//FTM2_IRQHandler
{
	FTM2->SC &= ~FTM_SC_TOF_MASK;
	PTE->PTOR|=1<<23;
	if (ftm_callback != 0)
	{
		ftm_callback();
	}
}


