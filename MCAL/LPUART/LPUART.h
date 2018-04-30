/* LPUART.h              (c) 2016 Freescale Semiconductor, Inc.
 * Descriptions: LPUART example code.
 * May 31 2016: Initial version
 */

#ifndef LPUART_H_
#define LPUART_H_

void LPUART1_init(void);
void LPUART1_transmit_char(char send);
void LPUART1_transmit_string(char data_string[]);
char LPUART1_receive_char(void);
void LPUART1_receive_and_echo_char(void);
#endif /* LPUART_H_ */
