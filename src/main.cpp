
#include "system.h" /* include peripheral declarations S32K148 */

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "Steering.h"
}


/* Extra declarations of ports to be used, not already under implementation*/
void Port_init_config(void)
{
	/* Enable clock for PORTs */
	PCC->PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTB_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK;

	/* Configure GPIO outputs for board LEDs */
	PORTE->PCR[21] = PORT_PCR_MUX(1);	// Red
	PORTE->PCR[22] = PORT_PCR_MUX(1);	// Green
	PORTE->PCR[23] = PORT_PCR_MUX(1);	// Blue
	PTE->PDDR |= (0b111<<21);

	/* Configure SW3 AND SW4 as input buttons */
	PORTC->PCR[12] = PORT_PCR_MUX(1)		// Port C12: MUX = GPIO
				   | PORT_PCR_PFE(1);		// 			 Input filter enabled
	PORTC->PCR[13] = PORT_PCR_MUX(1)		// Port C13: MUX = GPIO
				   | PORT_PCR_PFE(1);		// 			 Input filter enabled
	PTC->PDDR &= ~(0b11<<12);				// Data direction = input

	/* Cruise control driver */
	PORTC->PCR[29] = PORT_PCR_MUX(2);		// Port C29: FTM5_CH2		ENA
	PORTC->PCR[30] = PORT_PCR_MUX(1);		// Port C30: GPIO-output	IN1
	PORTC->PCR[31] = PORT_PCR_MUX(1);		// Port C31: GPIO-output	IN2
											// Port ###: GPIO-output	ENB/IN3
	PTC->PDDR |= (0b111<<29);
}


void LPIT0_init (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
}

/* Polling delay function */
void delay(double ms){
	  /*Channel 1*/
	  ms /=1000;
	  ms *= 40000000;
	  LPIT0->TMR[1].TVAL = (uint32_t) ms;
	  LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
	  while (0 == (LPIT0->MSR & LPIT_MSR_TIF1_MASK)) {}
	  LPIT0->MSR |= LPIT_MSR_TIF1_MASK;
}

int main()
{
	/* Clocks configuration and initialization */
	SOSC_init_8MHz();       /* Initialize system oscilator for 8 MHz xtal */
	SPLL_init_160MHz();     /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz();  /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */

	/* Testing section (Provisional stuff) ========================================================= */

//	Port_init_config();

	LPIT0_init();


	Steering_init();
	PWM_set_duty(M1_PWM, 0);
	GPIO_setPin(M1_INA);
	GPIO_setPin(M1_EN);

	float32_t pos = 0;
	float32_t set_point = 0;

	for(;;){
		pos = steering_encoder_read_deg();

		steering_manual_ctrl();
//		steering_set_position(set_point);
//		delay(50);

	}


	/* End of Testing section (Provisional stuff) ================================================== */

	return 0;
}

