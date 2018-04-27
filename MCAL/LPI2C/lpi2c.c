/*
 * lpi2c.c
 *
 *  Created on: Jun 9, 2016
 *      Author: B46911
 */

#include "lpi2c.h"

#if (defined(MCU_S32K142) || defined(MCU_S32K144) || defined(MCU_S32K146) || defined(MCU_S32K148))

void LPI2C_init(uint8_t pcc_index, LPI2C_Type* base){
    PCC->PCCn[pcc_index ] |= PCC_PCCn_PCS(1)        /* Clk src: System Oscillator*/
                              | PCC_PCCn_CGC_MASK;   /* Enable clock for LPI2C0 */

    base->MCFGR1 = LPI2C_MCFGR1_PRESCALE(0);	/*Prescaler selection*/

    base->MCCR0 = LPI2C_MCCR0_CLKLO(0x03)		/*Clock period low*/
                | LPI2C_MCCR0_CLKHI(0x04)		/*Clock period high*/
                | LPI2C_MCCR0_SETHOLD(0x03)		/*Setup hold delay*/
                | LPI2C_MCCR0_DATAVD(0x01);		/*Data valid delay*/

    base->MFCR = LPI2C_MFCR_TXWATER(0)
                  |LPI2C_MFCR_RXWATER(3);
    base->MCR |= LPI2C_MCR_MEN_MASK /* Enbale LPI2C as master */
            | LPI2C_MCR_DBGEN_MASK;
}



void LPI2C_Transmit (LPI2C_Type* base){
    base->MTDR = (0x05<<8)|((0x1E<<1)|0);
}

void wait(volatile int cycles){
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}

#endif

