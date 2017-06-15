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
odo_status oldStatus;

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
	float deltaThetaLeft;
	if(oldStatus.left.angle < -100 && currentStatus.left.angle > 100){
		deltaThetaLeft = 360.0 + oldStatus.left.angle - currentStatus.left.angle;
	}else if(oldStatus.left.angle > 100 && currentStatus.left.angle < -100){
		deltaThetaLeft = -(360.0 - oldStatus.left.angle + currentStatus.left.angle);
	}else{
		deltaThetaLeft = oldStatus.left.angle - currentStatus.left.angle;
	}
	float deltaSLeft = (deltaThetaLeft*M_PI/180.0) * (ODO_wheelDiameter / 2.0);
	float alphaLeft_rad = deltaSLeft/(ODO_halfAxialLength * 2.0);
	float deltaX = (cos(alphaLeft_rad)*ODO_halfAxialLength) - ODO_halfAxialLength;
	float deltaY = sin(alphaLeft_rad)*ODO_halfAxialLength;
	float cSlash = tan(alphaLeft_rad)*ODO_halfAxialLength;
	float deltaTheta_rad = atan2(ODO_halfAxialLength, cSlash) - M_PI_2;
	if(alphaLeft_rad >= M_PI || alphaLeft_rad <= -M_PI){
		deltaTheta_rad += M_PI;
	}
	float theta_rad = currentStatus.theta * M_PI/180.0;
	currentStatus.position.posX += roundf(cos(theta_rad)*deltaX-sin(theta_rad)*deltaY);
	currentStatus.position.posY += roundf(sin(theta_rad)*deltaX+cos(theta_rad)*deltaY);
	currentStatus.theta += deltaTheta_rad * 180.0 / M_PI;
	if(currentStatus.theta < -180.0){
		currentStatus.theta += 360.0;
	}else if(currentStatus.theta > 180){
		currentStatus.theta -= 360.0;
	}
	oldStatus = currentStatus;
}
