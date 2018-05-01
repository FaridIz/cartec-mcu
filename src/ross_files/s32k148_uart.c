#include "s32k148_uart.h"

#define BAUD 57600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)

LPUART_config_t ros_com = {
		.pin_rx.port = ePortC,
		.pin_rx.pin  = 6,
		.pin_rx.mux	 = eMux2,

		.pin_tx.port = ePortC,
		.pin_tx.pin  = 7,
		.pin_tx.mux	 = eMux2,

		.LPUART_instance 		= LPUART1,
		.ip_index		 		= PCC_LPUART1_INDEX,
		.LPUART_clock_source	= SOSCDIV2,
		.clock_src_freq			= 8000000,
		.Baudrate				= 57600
};

void s32k148_uart_init(void) {
	LPUART_init(ros_com);
}

void s32k148_uart_send_byte(uint8_t tx_byte) {
	LPUART_send(ros_com, tx_byte);
}

// returns -1 in the case of absence of data
int16_t s32k148_uart_receive_byte(void) {
	if(LPUART_readable(ros_com) == 1)
		return LPUART_get_uint8(ros_com);
	return -1;
}
