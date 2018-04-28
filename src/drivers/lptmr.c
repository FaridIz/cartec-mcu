/*
 * lptmr.c
 *
 *  Created on: Apr 8, 2016
 *      Author: B46911
 */

#include "lptmr.h"

#if (defined(MCU_S32K142) || defined(MCU_S32K144) || defined(MCU_S32K146) || defined(MCU_S32K148))


static vfcn_callback lptmr_callback = 0;


void LPTMR_init(uint16_t period_ms, vfcn_callback callback) {
  PCC->PCCn[PCC_LPTMR0_INDEX] = PCC_PCCn_CGC_MASK;    /* Enable LPTMR*/
  LPTMR0->PSR |= LPTMR_PSR_PCS(0b01)                  /* LPTMR clk src: 1KHz LPO*/
              | LPTMR_PSR_PBYP_MASK;               /* Bypass Prescaler */
  lptmr_callback = callback;                          /* save callback */
  LPTMR0->CMR = period_ms-1u;                         /* Interrupt in milliseconds*/
  LPTMR0->CSR |= LPTMR_CSR_TIE_MASK                    /* Enable Mask */
              | LPTMR_CSR_TEN_MASK;                   /* Enable Timer */
}

void LPTMR0_IRQHandler (void) {
  LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;  /* Clear flag */
  if (lptmr_callback != 0)
      lptmr_callback();
}

#endif


