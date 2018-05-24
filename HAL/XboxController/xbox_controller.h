/*
 * xbox_controller.h
 *
 *  Created on: 21/05/2018
 *      Author: Farid
 */

#ifndef XBOXCONTROLLER_XBOX_CONTROLLER_H_
#define XBOXCONTROLLER_XBOX_CONTROLLER_H_

#include "LPUART.h"

/* Library not implemented. X-box controller is implemented through ROS nodes*/

void xbox_controller_init(void);
void xbox_controller(float *steering_position, float *brake, float *throttle);


#endif /* XBOXCONTROLLER_XBOX_CONTROLLER_H_ */
