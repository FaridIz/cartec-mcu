
#include "system.h" /* include peripheral declarations S32K148 */
#include "test_loop.h"

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "FTM.h"
#include "ADC.h"
}

/* Extra declarations of ports to be used for testing*/
void Port_init_config(void)
{
	/* Enable clock for PORTs */
	PCC->PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTB_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK;

	/* Configure GPIO outputs for LEDs */
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


	/* Steering encoder */
	PORTA->PCR[1] = PORT_PCR_MUX(5); 		// Port  A1: FTM1_QD_PHA
	PORTB->PCR[2] = PORT_PCR_MUX(4); 		// Port  B2: FTM1_QD_PHB

	/* Driver VNH5019 */
	PORTB->PCR[3] = PORT_PCR_MUX(1);		// Port  B3: GPIO-output	M2 EN/DIAG
	PORTB->PCR[0] = PORT_PCR_MUX(6);		// Port  B0: FTM4_CH6		M2 PWM
	PTB->PDDR |= (0b1001);
	PORTA->PCR[30] = PORT_PCR_MUX(2);		// Port A30: FTM5_CH5		M1 PWM
	PORTA->PCR[31] = PORT_PCR_MUX(1);		// Port A31: GPIO-output	M2 INB
	PTA->PDDR |= (0b11<<30);
	PORTD->PCR[14] = PORT_PCR_MUX(1);		// Port D14: GPIO-output	M2 INA
	PORTD->PCR[15] = PORT_PCR_MUX(1);		// Port D15: GPIO-output	M1 EN/DIAG
	PORTD->PCR[17] = PORT_PCR_MUX(1);		// Port D17: GPIO-output	M1 INB
	PTD->PDDR |= (0b1011<<14);
	PORTC->PCR[11] = PORT_PCR_MUX(1);		// Port C11: GPIO-output	M1 INA
	PTC->PDDR |= (0b1<<11);

	/* Cruise control driver */
	PORTC->PCR[29] = PORT_PCR_MUX(2);		// Port C29: FTM5_CH2		ENA
	PORTC->PCR[30] = PORT_PCR_MUX(1);		// Port C30: GPIO-output	IN1
	PORTC->PCR[31] = PORT_PCR_MUX(1);		// Port C31: GPIO-output	IN2
											// Port ###: GPIO-output	ENB/IN3
	PTC->PDDR |= (0b111<<29);

	/* UART ROSserial */
	PORTC->PCR[6] = PORT_PCR_MUX(2);         /* Port  C6: LPUART1_RX */
	PORTC->PCR[7] = PORT_PCR_MUX(2);         /* Port  C7: LPUART1_TX */
}


/* Motor control section (Provisional stuff) ====================================================== */

uint32_t count = 0;


void count_revolutions(void){
	if (FTM1->QDCTRL & FTM_QDCTRL_TOFDIR_MASK) {
		count++;
	}
	else {
		count--;
	}
}

void Motor_init(void){
	FTM_QD_mode_Init(PCC_FTM1_INDEX, FTM1_Ovf_Reload_IRQn, FTM1, 4096, count_revolutions);
	FTM_PWM_mode_Init(PCC_FTM4_INDEX, FTM4); //10KHz cycle
}

double encoder_read(void){
	double temp = (double) FTM1->CNT;
	temp /= 4096;
	temp += (double) count;
	return temp;
}




/* End of Motor control section (Provisional stuff) ================================================ */

uint32_t temp_adc_func(void){
	convertAdcChan(0b101100);		/* Convert Channel AD28 to pot on EVB */
	while(adc_complete()==0){}      /* Wait for conversion complete flag */
	return read_adc_chx();   	/* Get channel's conversion results in mv */
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
//	Motor_init();
//	double pos = 0;
//	uint32_t adcval;

	test_setup();

	for(;;){
//		pos = encoder_read();
//		adcval = temp_adc_func();

		test_loop();

	}


	/* End of Testing section (Provisional stuff) ================================================== */

	return 0;
}

