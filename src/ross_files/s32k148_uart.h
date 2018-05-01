#ifndef _S32K148_UART_H_
#define _S32K148_UART_H_

#include "S32K148.h"
#include "s32k148_uart.h"
#include "LPUART.h"
#include "LPUART_config.h"


void s32k148_uart_init(void);
void s32k148_uart_send_byte(uint8_t tx_byte);
int16_t s32k148_uart_receive_byte(void);

#endif
