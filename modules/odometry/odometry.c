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
	currentStatus.theta = M_PI_2;
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
	// left wheel
	float deltaTheta;
	if(oldStatus.left.angle < -100 && currentStatus.left.angle > 100){
		deltaTheta = 360.0 + oldStatus.left.angle - currentStatus.left.angle;
	}else if(oldStatus.left.angle > 100 && currentStatus.left.angle < -100){
		deltaTheta = -(360.0 - oldStatus.left.angle + currentStatus.left.angle);
	}else{
		deltaTheta = oldStatus.left.angle - currentStatus.left.angle;
	}
	float deltaS = (deltaTheta*M_PI/180.0) * ODO_wheelRadius;
	float alpha_rad = deltaS/(ODO_halfAxialLength * 2.0);
	float deltaX = -(ODO_halfAxialLength - (cos(alpha_rad)*ODO_halfAxialLength));
	float deltaY = sin(alpha_rad)*ODO_halfAxialLength;
	float cSlash = tan(alpha_rad)*ODO_halfAxialLength;
	float deltaTheta_rad = -(atan2(ODO_halfAxialLength, cSlash) - M_PI_2);
	if(alpha_rad >= M_PI || alpha_rad <= -M_PI){
		deltaTheta_rad += M_PI;
	}
	float deltaXLeft_global = -sin(currentStatus.theta)*deltaX+cos(currentStatus.theta)*deltaY;
	float deltaYLeft_global = cos(currentStatus.theta)*deltaX-sin(currentStatus.theta)*deltaY;

	// right wheel
	if(oldStatus.right.angle < -100 && currentStatus.right.angle > 100){
		deltaTheta = 360.0 + oldStatus.right.angle - currentStatus.right.angle;
	}else if(oldStatus.right.angle > 100 && currentStatus.right.angle < -100){
		deltaTheta = -(360.0 - oldStatus.right.angle + currentStatus.right.angle);
	}else{
		deltaTheta = oldStatus.right.angle - currentStatus.right.angle;
	}
	deltaS = (deltaTheta*M_PI/180.0) * ODO_wheelRadius;
	alpha_rad = deltaS/(ODO_halfAxialLength * 2.0);
	deltaX = ODO_halfAxialLength - (cos(alpha_rad)*ODO_halfAxialLength);
	deltaY = sin(alpha_rad)*ODO_halfAxialLength;
	cSlash = tan(alpha_rad)*ODO_halfAxialLength;
	deltaTheta_rad += atan2(ODO_halfAxialLength, cSlash) - M_PI_2;
	if(alpha_rad >= M_PI || alpha_rad <= -M_PI){
		deltaTheta_rad += M_PI;
	}
	float deltaXRight_global = -sin(currentStatus.theta)*deltaX+cos(currentStatus.theta)*deltaY;
	float deltaYRight_global = cos(currentStatus.theta)*deltaX-sin(currentStatus.theta)*deltaY;

	currentStatus.position.posX -= deltaXLeft_global+deltaXRight_global;
	currentStatus.position.posY -= -deltaYLeft_global-deltaYRight_global;
	currentStatus.theta += deltaTheta_rad;
	if(currentStatus.theta < -M_PI){
		currentStatus.theta += 2.0*M_PI;
	}else if(currentStatus.theta > M_PI){
		currentStatus.theta -= 2.0*M_PI;
	}
	oldStatus = currentStatus;
}
