/*
 * CruiseControl.c
 *
 *  Created on: 14/05/2018
 *      Author: Farid
 */

#include "CruiseControl.h"

PORT_config_t IN1  = { .port = ePortC, .pin = 31, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t IN2  = { .port = ePortC, .pin = 30, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t EN_B = { .port = ePortE, .pin = 20, .mux = eMuxAsGPIO, .dir = eOutput };
PORT_config_t EN_A = { .port = ePortC, .pin = 29, .mux = eMux2,      .dir = eOutput };

