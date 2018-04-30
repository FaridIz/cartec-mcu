/*
 * LPUART.c
 *
 *  Created on: Apr 6, 2016
 *      Author: B46911
 */


#include <LPUART.h>

#if (defined(MCU_S32K142) || defined(MCU_S32K144) || defined(MCU_S32K146) || defined(MCU_S32K148))

/* Initialize LPUART, baud rate= 19200, 1 stop bit, 8 bit format, no parity*/
void LPUART_Init(uint8_t ip_index,LPUART_Type * LPUART_base)
{
    PCC->PCCn[ip_index] &= ~PCC_PCCn_CGC_MASK;    /* Ensure clk disabled for config */
    PCC->PCCn[ip_index] |= PCC_PCCn_PCS(0b001)    /* Clock Src= 1 (SOSCDIV2_CLK) */
                        |  PCC_PCCn_CGC_MASK;     /* Enable clock for LPUART regs */

    LPUART_base->BAUD &= ~LPUART_BAUD_SBR_MASK;
    LPUART_base->BAUD |= 52;    /* For 19200 baud: baud divisor=8M/19200/16 = ~26 */
    							/* SBR=26 */
    							/* OSR=16 */
    							/* SBNS=0: One stop bit */
    							/* RDMAE=0: DMA RX request disable */
    							/* TDMAE=0: DMA TX request disable */
    							/* M10=0: 7-bit to 9-bit data characters*/

    LPUART_base->CTRL = 0xC0000;    /* PE=0: No hw parity generation or checking */
    								/* M=0: 8-bit data characters*/
    								/* DOZEEN=0: LPUART enable in Doze mode */
    								/* RE=1: Receiver en */
    								/* TE=1: Transmitter en */
    								/* RIE=0 Rx int disabled */
    								/* TIE=0 Tx data reg empty int disabled */
    								/* TCIE=0 Tx complete int disabled */
}

void LPUART_Send(LPUART_Type * LPUART_base, uint8_t data)
{
    while((LPUART_base->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT==0); /* Wait for transmit buffer to be empty */
    LPUART_base->DATA=data;                      								  /* Send data */
}

void LPUART_transmit_string(LPUART_Type * LPUART_base, char data_string[])  {  /* Function to Transmit whole string */
  uint32_t i=0;
  while(data_string[i] != '\0')  {           /* Send chars one at a time */
    LPUART_Send(LPUART_base, data_string[i]);
    i++;
  }
}

#endif

