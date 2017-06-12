/*
 * odometry.c
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#include "odometry.h"

odo_angles angles;
odo_revs revolutions;

void odometry_init(void){
	sensor_init();
}

void odometry_updateAngles(void){
	sensor_getAngle_async(TLE_LEFT, &angles.angleWheel_l);
}

odo_angles odometry_getAngles(void){
	return angles;
}

void odometry_updateRevolutions(void){
	sensor_getRevolutions_async(TLE_LEFT, &revolutions.revsWheel_l);
}

odo_revs odometry_getRevolutions(void){
	return revolutions;
}
