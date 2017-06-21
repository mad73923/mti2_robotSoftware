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
	float deltaXLeft = -(ODO_halfAxialLength - (cos(alphaLeft_rad)*ODO_halfAxialLength));
	float deltaYLeft = sin(alphaLeft_rad)*ODO_halfAxialLength;
	float cSlashLeft = tan(alphaLeft_rad)*ODO_halfAxialLength;
	float deltaTheta_rad = -(atan2(ODO_halfAxialLength, cSlashLeft) - M_PI_2);
	if(alphaLeft_rad >= M_PI || alphaLeft_rad <= -M_PI){
		deltaTheta_rad += M_PI;
	}
	float deltaXLeft_global = -sin(currentStatus.theta)*deltaXLeft+cos(currentStatus.theta)*deltaYLeft;
	float deltaYLeft_global = cos(currentStatus.theta)*deltaXLeft-sin(currentStatus.theta)*deltaYLeft;
	// right wheel
	float deltaThetaRight;
	if(oldStatus.right.angle < -100 && currentStatus.right.angle > 100){
		deltaThetaRight = 360.0 + oldStatus.right.angle - currentStatus.right.angle;
	}else if(oldStatus.right.angle > 100 && currentStatus.right.angle < -100){
		deltaThetaRight = -(360.0 - oldStatus.right.angle + currentStatus.right.angle);
	}else{
		deltaThetaRight = oldStatus.right.angle - currentStatus.right.angle;
	}
	float deltaSRight = (deltaThetaRight*M_PI/180.0) * (ODO_wheelDiameter / 2.0);
	float alphaRight_rad = deltaSRight/(ODO_halfAxialLength * 2.0);
	float deltaXRight = ODO_halfAxialLength - (cos(alphaRight_rad)*ODO_halfAxialLength);
	float deltaYRight = sin(alphaRight_rad)*ODO_halfAxialLength;
	float cSlashRight = tan(alphaRight_rad)*ODO_halfAxialLength;
	deltaTheta_rad += atan2(ODO_halfAxialLength, cSlashRight) - M_PI_2;
	if(alphaRight_rad >= M_PI || alphaRight_rad <= -M_PI){
		deltaTheta_rad += M_PI;
	}
	float deltaXRight_global = -sin(currentStatus.theta)*deltaXRight+cos(currentStatus.theta)*deltaYRight;
	float deltaYRight_global = cos(currentStatus.theta)*deltaXRight-sin(currentStatus.theta)*deltaYRight;

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
