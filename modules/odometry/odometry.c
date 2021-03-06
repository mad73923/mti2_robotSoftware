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

void odometry_init_timer(void);
void updateAllCallback1(void);
void updateAllCallback2(void);

/*
 * Public functions
 */

void odometry_init(void){
	sensor_init();
	odometry_init_timer();
	odometry_autoupdate_start();
	currentStatus.theta = M_PI_2;
}

void odometry_updateStatus_async(void){
	sensor_getAngle_async(TLE_LEFT, &currentStatus.left.angle, updateAllCallback1);
}

odo_status odometry_getStatus(void){
	return currentStatus;
}

void odometry_setStatus(float x, float y, float theta){
	//TODO Disable interrupts?!
	currentStatus.position.posX = x;
	currentStatus.position.posY = y;
	currentStatus.theta = theta;
	oldStatus = currentStatus;
}

void odometry_autoupdate_start(void){
	LL_TIM_SetCounter(ODO_TIM_INST, 0);
	LL_TIM_EnableCounter(ODO_TIM_INST);
}

void odometry_autoupdate_stop(void){
	LL_TIM_DisableCounter(ODO_TIM_INST);
}

/*
 * Private functions
 */

void odometry_init_timer(void){
	ODO_TIM_CLK_INIT();
	LL_TIM_SetCounterMode(ODO_TIM_INST, LL_TIM_COUNTERMODE_UP);
	LL_TIM_SetPrescaler(ODO_TIM_INST, __LL_TIM_CALC_PSC(SystemCoreClock, 1000));
	LL_TIM_SetAutoReload(ODO_TIM_INST, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(ODO_TIM_INST), ODO_TIM_FREQ));

	LL_TIM_EnableIT_UPDATE(ODO_TIM_INST);
	NVIC_SetPriority(ODO_TIM_IRQN, ODO_TIM_PRIO);
	NVIC_EnableIRQ(ODO_TIM_IRQN);
}

void updateAllCallback1(void){
	sensor_getAngle_async(TLE_RIGHT, &currentStatus.right.angle, updateAllCallback2);
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
	currentStatus.left.speed = -deltaS*ODO_TIM_FREQ;
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
	deltaS = (-deltaTheta*M_PI/180.0) * ODO_wheelRadius;
	currentStatus.right.speed = -deltaS*ODO_TIM_FREQ;
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
	currentStatus.position.posY += deltaYLeft_global+deltaYRight_global;
	currentStatus.theta += deltaTheta_rad;
	if(currentStatus.theta < -M_PI){
		currentStatus.theta += 2.0*M_PI;
	}else if(currentStatus.theta > M_PI){
		currentStatus.theta -= 2.0*M_PI;
	}
	oldStatus = currentStatus;

	PID_trigger();
}

/*
 * Interrupt functions
 */

void ODO_TIM_IRQ_HANDLER(){
	LL_TIM_ClearFlag_UPDATE(ODO_TIM_INST);
	odometry_updateStatus_async();
}
