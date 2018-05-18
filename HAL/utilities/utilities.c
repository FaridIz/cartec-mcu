/*
 * utilities.c
 *
 *  Created on: 14/05/2018
 *      Author: Farid
 */

#include "utilities.h"


PORT_config_t LED_RED   = { .port = ePortE, .pin = 21, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t LED_GREEN = { .port = ePortE, .pin = 22, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t LED_BLUE  = { .port = ePortE, .pin = 23, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t SW3  = { .port = ePortC, .pin = 12, .mux = eMuxAsGPIO, .dir = eInput, .attrib.filter = ePassiveFilterEnabled};
PORT_config_t SW4  = { .port = ePortC, .pin = 13, .mux = eMuxAsGPIO, .dir = eInput, .attrib.filter = ePassiveFilterEnabled };

void LPIT0_init1 (void);

void utilities_init(void){
	GPIO_pinInit(LED_RED);
	GPIO_pinInit(LED_GREEN);
	GPIO_pinInit(LED_BLUE);
	GPIO_pinInit(SW3);
	GPIO_pinInit(SW4);
	ADC_init();	//12bit resolution

#ifdef BENCH_TOOLS
	LPIT0_init1();
#endif

	/* Initial state */
	GPIO_clearPin(LED_RED);
	GPIO_clearPin(LED_GREEN);
	GPIO_clearPin(LED_BLUE);
}

uint32_t utility_potentiometer_position(void){
	convertAdcChan(0b101100);		/* Convert Channel AD28 to pot on EVB */
	while(adc_complete()==0){}      /* Wait for conversion complete flag */
	return read_adc_chx();			/* Get channel's conversion results in mv (0-5000) */
}


#ifdef BENCH_TOOLS

void delay(float ms){
	  /*Channel 1*/
	  ms /=1000;
	  ms *= 40000000;
	  LPIT0->TMR[1].TVAL = (uint32_t) ms;
	  LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
	  while (0 == (LPIT0->MSR & LPIT_MSR_TIF1_MASK)) {}
	  LPIT0->MSR |= LPIT_MSR_TIF1_MASK;
}

void stopwatch(void){
	  /*Channel 1*/
	  LPIT0->TMR[1].TVAL = (uint32_t) 40000000;
	  LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
}

void LPIT0_init1 (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */

}

#endif

