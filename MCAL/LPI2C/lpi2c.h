/*
 * lpi2c.h
 *
 *  Created on: Jun 9, 2016
 *      Author: B46911
 */

#ifndef LPI2C_H_
#define LPI2C_H_

#include "device_registers.h" /* include peripheral declarations */

#if (defined(MCU_S32K142) || defined(MCU_S32K144) || defined(MCU_S32K146) || defined(MCU_S32K148))

void LPI2C_init(uint8_t pcc_index, LPI2C_Type* base);
void LPI2C_Transmit (LPI2C_Type* base);

#endif

#endif /* LPI2C_LPI2C_H_ */
