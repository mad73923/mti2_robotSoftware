/*
 * odometry.c
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#include "odometry.h"

odo_angles angles;

void odometry_init(void){
	sensor_init();
}

void odometry_updateAngles(void){
	sensor_getAngle_async(TLE_LEFT, &angles.angleWheel_l);
}

odo_angles odometry_getAngles(void){
	return angles;
}

odo_revs odometry_getRevolutions(void){
	odo_revs ret;
	ret.revsWheel_l = sensor_getRevolutions(TLE_LEFT);
	return ret;
}
