/*
 * GPIO.h
 *
 *  Created on: 23/04/2018
 *      Author: Farid
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

#include "system.h" /* include peripheral declarations */
#include "PORT_config.h"
#include "PORT.h"


void GPIO_pinInit(PORT_config_t config);
void GPIO_readPin(PORT_config_t config);
void GPIO_setPin(PORT_config_t config);
void GPIO_clearPin(PORT_config_t config);
void GPIO_togglePin(PORT_config_t config);



#endif /* GPIO_GPIO_H_ */
