/*
 * main.c       UART transmission simple example
 * 2017 Jul O Romero / Agustin Diaz - Initial version
 */

extern "C" {
#include "S32K148.h" /* include peripheral declarations S32K148 */
#include "clocks_and_modes.h"
#include "Break.h"
//#include "LPUART.h"
}

int data = 0;

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

//void Motor_setup (int Mdata, int Rdata, char fertig){
//	LPUART1_transmit_string("P0\n\r");     /* Transmit char string */
//	LPUART1_transmit_string("S0\n\r");     /* Transmit char string */
//
//	if (fertig == 0){
//		char M[] = {0x30 + Mdata/100, 0x30 + (Mdata/10)%10, 0x30 + Mdata%10}; //separate the reference variable into each digit for char array
//		char R[] = {0x30 + Rdata/1000, 0x30 + (Rdata/100)%10,
//					0x30 + (Rdata/10)%10, 0x30 + Rdata%10}; //separate the reference variable into each digit for char array
//
//		LPUART1_transmit_char('M');
//		LPUART1_transmit_char(M[0]);
//		LPUART1_transmit_char(M[1]);
//		LPUART1_transmit_char(M[2]);
//		LPUART1_transmit_string("\n\r");
//		LPUART1_transmit_char('R');
//		LPUART1_transmit_char(R[0]);
//		LPUART1_transmit_char(R[1]);
//		LPUART1_transmit_char(R[2]);
//		LPUART1_transmit_char(R[3]);
//		LPUART1_transmit_string("\n\r");
//	}else{
//		char R[] = {0x2D, 0x30 + Rdata/1000, 0x30 + (Rdata/100)%10,
//				0x30 + (Rdata/10)%10, 0x30 + Rdata%10};
//		LPUART1_transmit_char('R');
//		LPUART1_transmit_char(R[0]);
//		LPUART1_transmit_char(R[1]);
//		LPUART1_transmit_char(R[2]);
//		LPUART1_transmit_char(R[3]);
//		LPUART1_transmit_char(R[4]);
//		LPUART1_transmit_string("\n\r");
//	}
//}

int main(void)
{
    WDOG_disable();        /* Disable WDGO*/
    SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
    SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
    NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
    PORT_init();           /* Configure ports */

    LPUART1_init();        /* Initialize LPUART @ 9600*/

    int Mdata = 50; //reference of 'max speed' for break motor
    int Rdata = 450; //reference of 'go to angle' for break motor
    char fertig = 1; //variable to check if breaking has finished or not
    Motor_setup(Mdata,Rdata,fertig);

    for(;;) {
        //LPUART1_transmit_char('>');  		/* Transmit prompt character*/
        //LPUART1_receive_and_echo_char();	/* Wait for input char, receive & echo it*/
		//LPUART1_transmit_string("S100");     /* Transmit char string */
    }
}


