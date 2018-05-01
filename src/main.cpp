
#include "system.h" /* include peripheral declarations S32K148 */
#include "ross_files/ros_lib/std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include "ros.h"

// Include C headers (ie, non C++ headers) in this block
extern "C" {
#include "clocks_and_modes.h"
#include "Steering.h"
#include "LPUART.h"
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

// Function prototypes

void PORT_init_dummy (void);
void LPIT0_init (void);
uint8_t LPUART1_receive_char_dummy(void);
void send_to_matlab(uint16_t data);
void delay(double ms); /* Polling delay function */


int main() {
  /* Clocks configuration and initialization */
  SOSC_init_8MHz();       /* Initialize system oscilator for 8 MHz xtal */
  SPLL_init_160MHz();     /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
  NormalRUNmode_80MHz();  /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */

  /* Testing section (Provisional stuff) ========================================================= */

  LPIT0_init();
  Steering_init();
  PWM_set_duty(M1_PWM, 0);
  GPIO_setPin(M1_INA);
  GPIO_setPin(M1_EN);


  PORT_init_dummy();
  LPUART_Init(PCC_LPUART1_INDEX, LPUART1);

  int16_t input = 0;
  uint16_t angle = 0;

  for(;;){

  	input = ((int16_t) LPUART1_receive_char_dummy() - 127) * 2;

  	if(input>=0) {
  		set_direction(CW);
  		PWM_set_duty(M1_PWM, input);
  	}
  	else {
  		set_direction(CCW);
  		PWM_set_duty(M1_PWM, -input);
  	}

  	angle = (uint16_t) (steering_encoder_read_rev()*4096 + 30000);
  	send_to_matlab(angle);
  }
  return 0;
}

uint8_t LPUART1_receive_char_dummy(void) {    /* Function to Receive single Char */
  uint8_t recieve;
  while((LPUART1->STAT & LPUART_STAT_RDRF_MASK)>>LPUART_STAT_RDRF_SHIFT==0);
                                     /* Wait for received buffer to be full */
  recieve= LPUART1->DATA;            /* Read received data*/
  return recieve;
}
void send_to_matlab(uint16_t data) {
  LPUART_Send(LPUART1, (uint8_t) 'A');
  uint8_t a = (uint8_t) (data & 0xff);
  uint8_t b = (uint8_t) (data >> 8);

  LPUART_Send(LPUART1, a);
  LPUART_Send(LPUART1, b);

  LPUART_Send(LPUART1, 0);
}

void delay(double ms) {
  /*Channel 1*/
  ms /=1000;
  ms *= 40000000;
  LPIT0->TMR[1].TVAL = (uint32_t) ms;
  LPIT0->TMR[1].TCTRL = 0x00000001; //Enable
  while (0 == (LPIT0->MSR & LPIT_MSR_TIF1_MASK)) {}
  LPIT0->MSR |= LPIT_MSR_TIF1_MASK;
}

void LPIT0_init (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
}

void PORT_init_dummy (void) {
  PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
  PORTC->PCR[6]|=PORT_PCR_MUX(2);           /* Port C6: MUX = ALT2,UART1 RX */
  PORTC->PCR[7]|=PORT_PCR_MUX(2);           /* Port C7: MUX = ALT2,UART1 TX */
}
