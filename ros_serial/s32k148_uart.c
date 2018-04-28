#include "S32K148.h"
#include "s32k148_uart.h"

#include "../drivers/LPUART.h"


#define BAUD 57600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)


void s32k148_uart_init(void) {
	PCC->PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC(1);
	PORTC->PCR[6]|=PORT_PCR_MUX(2);           /* Port C6: MUX = ALT2,UART1 RX */
	PORTC->PCR[7]|=PORT_PCR_MUX(2);           /* Port C7: MUX = ALT2,UART1 TX */
	LPUART_Init(PCC_LPUART1_INDEX, LPUART1); // (ip_index, base)
}

void s32k148_uart_send_byte(uint8_t tx_byte) {
	LPUART_Send(LPUART1, tx_byte);
}

// returns -1 in the case of absence of data
int16_t s32k148_uart_receive_byte(void) {
	return LPUART_Receive();
}
