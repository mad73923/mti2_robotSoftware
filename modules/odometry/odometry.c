/*
 * odometry.c
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#include "odometry.h"

/*
 * Variables
 */

odo_status currentStatus;

uint8_t updateEndless;

/*
 * Private functions prototypes
 */

void updateAllCallback1(void);
void updateAllCallback2(void);

/*
 * Public functions
 */

void odometry_init(void){
	sensor_init();
}

void odometry_updateStatus_async(void){
	sensor_getAngle_async(TLE_LEFT, &currentStatus.left.angle, updateAllCallback1);
}

odo_status odometry_getStatus(void){
	return currentStatus;
}

/*
 * Private functions
 */

void updateAllCallback1(void){
	sensor_getRevolutions_async(TLE_LEFT, &currentStatus.left.revolutions, updateAllCallback2);
}

void updateAllCallback2(void){
	sensor_getSpeed_async(TLE_LEFT, &currentStatus.left.speed, 0);
}
