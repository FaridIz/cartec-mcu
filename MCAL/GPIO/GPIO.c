/*
 * GPIO.c
 *
 *  Created on: 23/04/2018
 *      Author: Farid
 */


#include "GPIO.h"

void GPIO_pinInit(PORT_config_t config)
{
	/* Initialization of port */
	PORT_init(config);
	/* Get proper GPIO pointer */
	GPIO_Type *gpio_ptrs[] = GPIO_BASE_PTRS;
	/* Set Port pin direction */
	if (config.dir == eInput)
		gpio_ptrs[config.port]->PDDR &= ~(1 << config.pin);
	else if (config.dir == eOutput)
		gpio_ptrs[config.port]->PDDR |= (1 << config.pin);
}

uint8_t GPIO_readPin(PORT_config_t config)
{
	/* Get proper GPIO pointer */
	GPIO_Type *gpio_ptrs[] = GPIO_BASE_PTRS;
	/* Read Pin state*/
	return (gpio_ptrs[config.port]->PDIR & (1 << config.pin)) >> config.pin;
}

void GPIO_setPin(PORT_config_t config)
{
	/* Get proper GPIO pointer */
	GPIO_Type *gpio_ptrs[] = GPIO_BASE_PTRS;
	/* Set Pin */
	gpio_ptrs[config.port]->PSOR |= (1 << config.pin);
}

void GPIO_clearPin(PORT_config_t config)
{
	/* Get proper GPIO pointer */
	GPIO_Type *gpio_ptrs[] = GPIO_BASE_PTRS;
	/* Clear Pin */
	gpio_ptrs[config.port]->PCOR |= (1 << config.pin);
}

void GPIO_togglePin(PORT_config_t config)
{
	/* Get proper GPIO pointer */
	GPIO_Type *gpio_ptrs[] = GPIO_BASE_PTRS;
	/* Toggle Pin */
	gpio_ptrs[config.port]->PTOR |= (1 << config.pin);
}

