/*
 * main.c       UART transmission simple example
 * 2017 Jul O Romero / Agustin Diaz - Initial version
 */

extern "C" {
#include "S32K148.h" /* include peripheral declarations S32K148 */
#include "clocks_and_modes.h"
#include "Break.h"
}

int Mdata = 50; //reference of 'max speed' for break motor
int Rdata = 90; //reference of 'go to angle' for break motor
char fertig = 0; //variable to check if breaking has finished or not

void PORT_init (void) {
    PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
    PORTC->PCR[6]|=PORT_PCR_MUX(2);           /* Port C6: MUX = ALT2,UART1 TX */
    PORTC->PCR[7]|=PORT_PCR_MUX(2);           /* Port C7: MUX = ALT2,UART1 RX */
}

void WDOG_disable (void){
    WDOG->CNT=0xD928C520;     /* Unlock watchdog */
    WDOG->TOVAL=0x0000FFFF;   /* Maximum timeout value */
    WDOG->CS = 0x00002100;    /* Disable watchdog */
}

/*####################################################################################*/
void LPIT0_init (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
//  LPIT0->MIER=LPIT_MIER_TIE0_MASK;  /* Enable timer interrupt channel 0*/
}

void delay(double ms){
	  ms /=1000;
	  ms *= 40000000;
	  LPIT0->TMR[1].TVAL = (uint32_t) ms;
	  LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
	  while (0 == (LPIT0->MSR & LPIT_MSR_TIF1_MASK)) {}
	  LPIT0->MSR |= LPIT_MSR_TIF1_MASK;
}
/*####################################################################################*/

int main(void)
{
    WDOG_disable();        /* Disable WDGO*/
    SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
    SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
    NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
    PORT_init();           /* Configure ports */
    LPIT0_init();

    LPUART1_init();        /* Initialize LPUART @ 9600*/

    //Motor_setup(Mdata,Rdata*5,fertig);

    for(;;) {
        Motor_setup(Mdata,Rdata*5,fertig);
        delay(1000);
    }
}


