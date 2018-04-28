#ifndef _S32K148_UART_H_
#define _S32K148_UART_H_

void s32k148_uart_init(void);
void s32k148_uart_send_byte(uint8_t tx_byte);
int16_t s32k148_uart_receive_byte(void);

#endif
