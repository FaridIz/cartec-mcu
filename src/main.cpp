
#include "system.h" /* include peripheral declarations S32K148 */

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "ADC.h"
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

/* Temporary ADC function for testing */
uint32_t temporary_adc_func(void){
	convertAdcChan(0b101100);		/* Convert Channel AD28 to pot on EVB */
	while(adc_complete()==0){}      /* Wait for conversion complete flag */
	return read_adc_chx();			/* Get channel's conversion results in mv */
}


int main()
{
	/* Clocks configuration and initialization */
	SOSC_init_8MHz();       /* Initialize system oscilator for 8 MHz xtal */
	SPLL_init_160MHz();     /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz();  /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */

	/* Testing section (Provisional stuff) ========================================================= */

//	Port_init_config();
//	ADC_init();            /* Init ADC resolution 12 bit*/
//	uint32_t adcval;

	double pos = 0;
	Steering_init();
	PWM_set_duty(M1_PWM, 0);
	GPIO_setPin(M1_INA);
	GPIO_setPin(M1_EN);

	for(;;){
		pos = steering_encoder_read();
//		adcval = temporary_adc_func();

	}


	/* End of Testing section (Provisional stuff) ================================================== */

	return 0;
}

