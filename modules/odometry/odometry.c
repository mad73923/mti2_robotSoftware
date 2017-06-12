/*
 * odometry.c
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#include "odometry.h"

odo_status currentStatus;

void odometry_init(void){
	sensor_init();
}

void odometry_updateStatus_async(void){
	sensor_updateAll_async(&currentStatus.left.angle, &currentStatus.left.revolutions);
}

odo_status odometry_getStatus(void){
	return currentStatus;
}
