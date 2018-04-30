/*
 * main.c       UART transmission simple example
 * 2017 Jul O Romero / Agustin Diaz - Initial version
 */

extern "C" {
#include "S32K148.h" /* include peripheral declarations S32K148 */
#include "clocks_and_modes.h"
//#include "Break.h"
#include "LPUART.h"
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

void Motor_setup (char Mstring[], char Rstring[]){
    LPUART1_transmit_string("P0\n\r");     /* Transmit char string */
    LPUART1_transmit_string("S0\n\r");     /* Transmit char string */
//    LPUART1_transmit_string(Mstring);     /* Transmit char string */
	LPUART1_transmit_char('M');
	LPUART1_transmit_char(Mstring[0]);
	LPUART1_transmit_char(Mstring[1]);
	LPUART1_transmit_string("\n\r");
//    LPUART1_transmit_string(Rstring);     /* Transmit char string */
	LPUART1_transmit_char('R');
	LPUART1_transmit_char(Rstring[0]);
	LPUART1_transmit_char(Rstring[1]);
	LPUART1_transmit_char(Rstring[2]);
	LPUART1_transmit_char(Rstring[3]);
	LPUART1_transmit_string("\n\r");
}

int main(void)
{
    WDOG_disable();        /* Disable WDGO*/
    SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
    SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
    NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
    PORT_init();           /* Configure ports */

    LPUART1_init();        /* Initialize LPUART @ 9600*/

    //char newline[] = "\n\r";
    int dat = 50;
    char M[] = {dat};
    char R[] = "1800";
    Motor_setup(M,R);

//    LPUART1_transmit_string("Input character to echo...\n\r"); /* Transmit char string */

    for(;;) {
        //LPUART1_transmit_char('>');  		/* Transmit prompt character*/
        //LPUART1_receive_and_echo_char();	/* Wait for input char, receive & echo it*/
		//LPUART1_transmit_string("S100");     /* Transmit char string */
    }
}


