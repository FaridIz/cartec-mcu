/*
 * Scheduler.c
 *
 *  Created on: 01/05/2018
 *      Author: Farid
 */

#include "Scheduler.h"

uint32_t steurung = 0;

scheduler_task_config_t * task_array;
uint8_t task_array_size = 0;


void NVIC_init_IRQs(void);
void LPIT0_init(uint32_t scheduler_ticks);


void scheduler_init(scheduler_task_config_t * scheduler_task_array, uint8_t number_of_tasks ,uint32_t step_ticks){

	task_array = scheduler_task_array;
	task_array_size = number_of_tasks;

	/* Initialize configurations for Timer module and corresponding interrupt */
	NVIC_init_IRQs();
	LPIT0_init(step_ticks);
}


void scheduler(void){
	steurung++;
	int i;
	for(i=0; i<task_array_size; i++){
		if(steurung == task_array[i].start_tick){
			task_array[i].task_callback();
			task_array[i].start_tick += task_array[i].period_ticks;
		}
	}
}


void LPIT0_Ch0_IRQHandler (void){
	LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */
	scheduler();
}

/* ====================================================================================================== */

void NVIC_init_IRQs(void) {
    S32_NVIC->ICPR[1] = 1 << (LPIT0_Ch0_IRQn % 32);  /* IRQ48-LPIT0 ch0: clr any pending IRQ*/
    S32_NVIC->ISER[1] = 1 << (LPIT0_Ch0_IRQn % 32);  /* IRQ48-LPIT0 ch0: enable IRQ */
    S32_NVIC->IP[LPIT0_Ch0_IRQn] = 0xA0;             /* IRQ48-LPIT0 ch0: priority 10 of 0-15*/
}

void LPIT0_init(uint32_t ticks) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
                              /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
                              /* SW_RST=0: SW reset does not reset timer chans, regs */
                           	  /* M_CEN=1: enable module clk (allows writing other LPIT0 regs)*/

  LPIT0->MIER=LPIT_MIER_TIE0_MASK;  /* Enable timer interrupt channel 0*/

  LPIT0->TMR[0].TVAL = ticks;    /* Chan 0 Timeout period: 40M clocks */
  LPIT0->TMR[0].TCTRL = 0x00000001; /* T_EN=1: Timer channel is enabled */
                              /* CHAIN=0: channel chaining is disabled */
                              /* MODE=0: 32 periodic counter mode */
                              /* TSOT=0: Timer decrements immediately based on restart */
                              /* TSOI=0: Timer does not stop after timeout */
                              /* TROT=0 Timer will not reload on trigger */
                              /* TRG_SRC=0: External trigger soruce */
                              /* TRG_SEL=0: Timer chan 0 trigger source is selected*/
}
