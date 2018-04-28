/*
 * lpit.h
 *
 *  Created on: Apr 8, 2016
 *      Author: B46911
 */

#ifndef LPTMR_H_
#define LPTMR_H_

#include "device_registers.h" /* include peripheral declarations */

#if (defined(MCU_S32K142) || defined(MCU_S32K144) || defined(MCU_S32K146) || defined(MCU_S32K148))

typedef void (*vfcn_callback)(void);

void LPTMR_init(uint16_t period_ms, vfcn_callback callback);

#endif

#endif /* LPTMR_H_ */
