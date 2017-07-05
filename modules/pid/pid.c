/*
 * pid.c
 *
 *  Created on: 05.07.2017
 *      Author: matthias
 */

#include "pid.h"

volatile int32_t deltaT = 1;

volatile int32_t Kp = 1;
volatile int32_t Ki = 10;
volatile int32_t Kd;

volatile int32_t targetValueLeft = 500;

volatile int32_t inputValue;
volatile int32_t inputValueOld;
volatile int32_t inputValueOld2;

volatile int32_t controlValue;
volatile int32_t controlValueOld;

volatile int32_t q0, q1, q2;

static int32_t calculateControlValue(void);


void PID_trigger(void){
	controlValue = calculateControlValue();
	controlValue = motor_setSpeed(MOTORLEFT, controlValue);

	controlValueOld = controlValue;
	inputValueOld2 = inputValueOld;
	inputValueOld = inputValue;
}

int32_t calculateControlValue(void){
	odo_status stat = odometry_getStatus();
	inputValue = targetValueLeft - stat.left.speed;
	q0 = (Kp + (Ki*deltaT) + (Kd/deltaT));
	q1 = -Kp -2*(Kd/deltaT);
	q2 = Kd/deltaT;
	volatile int32_t erg = controlValueOld;

	erg += (q0*inputValue/100);
	erg += (q1*inputValueOld/100);
	erg += (q2*inputValueOld2/100);

	return erg;
}
