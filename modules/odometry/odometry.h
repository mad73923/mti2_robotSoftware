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

void odometry_init(void);
odo_angles odometry_getAngles(void);


#endif /* ODOMETRY_ODOMETRY_H_ */
