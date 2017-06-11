/*
 * odometry.h
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#ifndef ODOMETRY_ODOMETRY_H_
#define ODOMETRY_ODOMETRY_H_

#include "TLE5012B.h"

typedef struct{
	float angleWheel_l;
}odo_angles;

typedef struct{
	int16_t revsWheel_l;
}odo_revs;

void odometry_init(void);
odo_angles odometry_getAngles(void);
odo_revs odometry_getRevolutions(void);

void odometry_updateAngles(void);

#endif /* ODOMETRY_ODOMETRY_H_ */
