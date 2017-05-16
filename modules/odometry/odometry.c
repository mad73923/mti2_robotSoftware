/*
 * odometry.c
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#include "odometry.h"

void odometry_init(void){
	sensor_init();
}

odo_angles odometry_getAngles(void){
	odo_angles ret;
	ret.angleWheel_l = sensor_getAngle(TLE_LEFT);
	return ret;
}
