#include "s32k148_time.h"
#include "S32K148.h"

#include "../drivers/LPTMR.h"


#define LPTMR_OVERFLOW_PERIOD_MS 10


volatile static uint32_t overflow = 0;


//Callback function for LPTMR0 interrupt handler
void overflow_counter(void) {
  overflow++;
}

// Enable LPTMR0 interrupt
void NVIC_init_LPTMR0_IRQs (void) {
  S32_NVIC->ICPR[1] = 1 << (LPTMR0_IRQn % 32);  /* IRQ58-LPTMR: clr any pending IRQ*/
  S32_NVIC->ISER[1] = 1 << (LPTMR0_IRQn % 32);  /* IRQ58-LPTMR: enable IRQ */
  S32_NVIC->IP[LPTMR0_IRQn] = 0xA0;             /* IRQ58-LPTMR: priority 10 of 0-15*/
}

// Initialize LPTMR0 16-bit timer @ 1KHz
void s32k148_time_init(void) {
  LPTMR_init(LPTMR_OVERFLOW_PERIOD_MS, overflow_counter);
  NVIC_init_LPTMR0_IRQs();
}

// Get the current time in milliseconds
uint32_t s32k148_time_now(void) {
  return overflow * LPTMR_OVERFLOW_PERIOD_MS;
}


