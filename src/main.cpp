/*
 * main.c       UART transmission simple example
 * 2017 Jul O Romero / Agustin Diaz - Initial version
 */

extern "C" {
#include "S32K148.h" /* include peripheral declarations S32K148 */
#include "clocks_and_modes.h"
#include "Break.h"
}

int Mdata = 25; //reference of 'max speed' for break motor
int Rdata = 90; //reference of 'go to angle' for break motor
char fertig = 0; //variable to check if breaking has finished or not

void PORT_init (void) {
	PCC->PCCn[PCC_PORTA_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTA */
	PORTA->PCR[8]|=PORT_PCR_MUX(2);           /* Port A8: MUX = ALT2,UART1 TX */
	PORTA->PCR[9]|=PORT_PCR_MUX(2);           /* Port A9: MUX = ALT2,UART1 RX */
}

void WDOG_disable (void){
    WDOG->CNT=0xD928C520;     /* Unlock watchdog */
    WDOG->TOVAL=0x0000FFFF;   /* Maximum timeout value */
    WDOG->CS = 0x00002100;    /* Disable watchdog */
}

int main(void)
{
    WDOG_disable();        /* Disable WDGO*/
    SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
    SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
    NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
    PORT_init();           /* Configure ports */
    LPUART_Init(PCC_LPUART2_INDEX, LPUART2);        /* Initialize LPUART @ 9600*/

    Motor_init();
    Motor_cmds(Mdata,Rdata*5,fertig);

    for(;;) {

    }
}


