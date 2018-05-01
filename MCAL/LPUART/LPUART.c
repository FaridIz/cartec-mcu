/*
 * LPUART.c
 *
 *  Created on: Apr 6, 2016
 *      Author: B46911
 */


#include "LPUART.h"


#if (defined(MCU_S32K142) || defined(MCU_S32K144) || defined(MCU_S32K146) || defined(MCU_S32K148))


void LPUART_init(LPUART_config_t config) {
	/* Port initialization configuration */
	PORT_init(config.pin_rx);
	PORT_init(config.pin_tx);

	/* Select and enable, clock for LPUART */
    PCC->PCCn[config.ip_index] &= ~PCC_PCCn_CGC_MASK;    					/* Ensure clk disabled for config */
    PCC->PCCn[config.ip_index] |= PCC_PCCn_PCS(config.LPUART_clock_source)  /* Clock Src= 1 (SOSCDIV2_CLK) */
                        	   |  PCC_PCCn_CGC_MASK;     					/* Enable clock for LPUART regs */

    /* Calculate proper BAUD_SBR value */
    float sbr = (float) config.clock_src_freq;
    sbr /= (DEFAULT_OSR * config.Baudrate);
    sbr *= 10;
    if( ((uint16_t)sbr % 10) >= 5 ){
    	sbr += 10;
    }
    sbr /= 10;

    /* Clear SBR and set OSR to default value (01111b), oversampling ratio of 16 */
    config.LPUART_instance->BAUD &= ~LPUART_BAUD_SBR_MASK;

    /* Set SBR to get specified baudrate */
    config.LPUART_instance->BAUD |= ((uint16_t)sbr) & LPUART_BAUD_SBR_MASK;
    	/*When clock_src_freq = 80MHz: 26 for 19200, 9 for 57600 */
    	/* SBNS=0: One stop bit */
    	/* RDMAE=0: DMA RX request disable */
    	/* TDMAE=0: DMA TX request disable */
    	/* M10=0: 7-bit to 9-bit data characters*/

    /* Other configurations */
    config.LPUART_instance->CTRL = 0xC0000;     /* PE=0: No hw parity generation or checking */
    	/* M=0: 	 8-bit data characters*/
    	/* DOZEEN=0: LPUART enable in Doze mode */
    	/* RE=1: 	 Receiver en */
    	/* TE=1: 	 Transmitter en */
    	/* RIE=0 	 Rx int disabled */
    	/* TIE=0 	 Tx data reg empty int disabled */
    	/* TCIE=0 	 Tx complete int disabled */
}


void LPUART_send(LPUART_config_t config, uint8_t data) {
	/* Wait for transmit buffer to be empty */
    while(( (config.LPUART_instance->STAT & LPUART_STAT_TDRE_MASK) >> LPUART_STAT_TDRE_SHIFT ) == 0);
    /* Send data */
    config.LPUART_instance->DATA = data;
}

void LPUART_transmit_string(LPUART_config_t config, uint8_t data_string[])  {  /* Function to Transmit whole string */
  uint16_t i=0;
  while(data_string[i] != '\0')  {           /* Send chars one at a time */
    LPUART_send(config, data_string[i]);
    i++;
  }
}

int8_t LPUART_readable(LPUART_config_t config){
	if(config.LPUART_instance->STAT & LPUART_STAT_RDRF_MASK)
		return 1;
	else
		return -1;
}

int16_t LPUART_receive(LPUART_config_t config) {
	// Quick fix to clean overrun flag
	// TODO Fix this a better way
	if (config.LPUART_instance->STAT & LPUART_STAT_OR_MASK)
		config.LPUART_instance->STAT |= LPUART_STAT_OR_MASK;

	if (config.LPUART_instance->STAT & LPUART_STAT_RDRF_MASK)
	  return config.LPUART_instance->DATA;
	else
	  return -1;
}

uint8_t LPUART_get_uint8(LPUART_config_t config){
	if (config.LPUART_instance->STAT & LPUART_STAT_OR_MASK)
		config.LPUART_instance->STAT |= LPUART_STAT_OR_MASK;
	/* Wait for received buffer to be full */
	while((config.LPUART_instance->STAT & LPUART_STAT_RDRF_MASK)>>LPUART_STAT_RDRF_SHIFT == 0);
	/* Read received data*/
	return config.LPUART_instance->DATA;
}


#endif
