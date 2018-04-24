/*
 * test_loop.cpp
 *
 *  Created on: 23/04/2018
 *      Author: Farid
 */

#include "system.h"
#include "test_loop.h"

extern "C" {
#include "GPIO.h"
#include "dual_vnh5019_config.h"
}

void LPIT0_init (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
                              /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
                              /* SW_RST=0: SW reset does not reset timer chans, regs */
                           	  /* M_CEN=1: enable module clk (allows writing other LPIT0 regs)*/

  LPIT0->MIER=LPIT_MIER_TIE0_MASK;  /* Enable timer interrupt channel 0*/
}
void delay(double ms){
	  /*Channel 1*/
	  ms /=1000;
	  ms *= 40000000;
	  LPIT0->TMR[1].TVAL = (uint32_t) ms;
	  LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
	  while (0 == (LPIT0->MSR & LPIT_MSR_TIF1_MASK)) {}
	  LPIT0->MSR |= LPIT_MSR_TIF1_MASK;

}


void test_setup(void){
	GPIO_pinInit(dummy);
	LPIT0_init();
}

void test_loop(void){
	GPIO_setPin(dummy);
	delay(250);
	GPIO_togglePin(dummy);
	delay(250);

}
