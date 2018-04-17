
#include "S32K148.h" /* include peripheral declarations S32K148 */


// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "FTM.h"
}

/* Extra declarations of ports to be used for testing*/
void extras(void)
{
	/* Enable clock for PORTA */
	PCC->PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC_MASK;
	/* Configure ports to be used by FTM2*/
	PORTA->PCR[12] = PORT_PCR_MUX(6); // Set PTA12 for FTM2 - Phase B input
	PORTA->PCR[13] = PORT_PCR_MUX(6); // Set PTA13 for FTM2 - Phase A input

	/* Enable clock for PORTE */
	PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK;
	/* Configure PTE23 as output for Blue LED */
	PORTE->PCR[23] = PORT_PCR_MUX(1);
	PTE->PDDR=1<<23;
}


/* Motor control section (Provisional stuff) ====================================================== */
double count = 0;

void count_revolutions(void){
	if (FTM2->QDCTRL & FTM_QDCTRL_TOFDIR_MASK){
		count++;
	}
	else{
		count--;
	}
}

double position(void){
	double temp = (double) FTM2->CNT;
	return count + (temp/4096);
}
/* End of Motor control section (Provisional stuff) ================================================ */




int main()
{
	/* Clocks configuration and initialization */
	SOSC_init_8MHz();       /* Initialize system oscilator for 8 MHz xtal */
	SPLL_init_160MHz();     /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz();  /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */


	/* Testing section (Provisional stuff) ========================================================= */
	extras();
	FTM_QD_mode_Init(PCC_FTM2_INDEX, FTM2_Ovf_Reload_IRQn, FTM2, 4096, count_revolutions);

	double pos = 0;
	for(;;){
		pos = position();
	}


	/* End of Testing section (Provisional stuff) ================================================== */




	/* to avoid the warning message for GHS: statement is unreachable*/
#if defined (__ghs__)
#pragma ghs nowarning 111
#endif
#if defined (__ICCARM__)
#pragma diag_suppress=Pe111
#endif
	return 0;
}




