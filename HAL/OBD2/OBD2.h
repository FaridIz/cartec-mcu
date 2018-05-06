/*
 * OBD2.h
 *
 *  Created on: 02/05/2018
 *      Author: Farid
 */

#ifndef OBD2_OBD2_H_
#define OBD2_OBD2_H_

#include <LPUART.h>

extern LPUART_config_t OBD2;

#define BUFFER_SIZE 40

/* Mode 1 PIDs */
#define PID_RPM 0x0C	// 2 bytes de respuesta
#define PID_SPEED 0x0D	// 1 byte  de respuesta
#define PID_TPS 0x11	// 1 byte  de respuesta


typedef enum {
	PROTO_AUTO = 0,
	PROTO_SAE_J1850_PWM = 1,
	PROTO_SAE_J1850_VPW = 2,
    PROTO_ISO_9141_2 = 3,
    PROTO_KWP2000_5KBPS = 4,
    PROTO_KWP2000_FAST = 5,
    PROTO_CAN_11B_500K = 6,
    PROTO_CAN_29B_500K = 7,
    PROTO_CAN_29B_250K = 8,
    PROTO_CAN_11B_250K = 9,
} OBD_PROTOCOLS;

void obd2_init(void);

#endif /* OBD2_OBD2_H_ */


/* Notes:
 * "[...] warning when trying to optimize the speed at which you obtain information
 * from vehicles. Prior to the APR2002 release of the J1979 standard, sending J1850
 * requests more frequently than every 100 msec was forbidden. With the APR202
 * update, scan tools were allowed to send the next request without delay if it was
 * determined that all the responses to the previous request had been received.
 * Vehicles made prior to this time may not be able to tolerate requests at too
 * fast rate, so use caution with them"
 * Retrieved from ELM327 Datasheet, section "Talking to the vehicle", p.32.
 * www.elmelectronics.com
 */


