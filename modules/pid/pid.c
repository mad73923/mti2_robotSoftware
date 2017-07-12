/*
 * pid.c
 *
 *  Created on: 05.07.2017
 *      Author: matthias
 */

#include "pid.h"

volatile int32_t deltaT = 1;

volatile int32_t Kp = 30;
volatile int32_t Ki = 20;
volatile int32_t Kd = 8;

struct {
	int32_t targetValueLeft;
	int32_t targetValueRight;
} targetValues;

inputValues left_input;
inputValues right_input;

controlValues left_control;
controlValues right_control;

volatile int32_t q0, q1, q2;

volatile uint8_t pid_isActive;

/*
 * Private functions prototype
 */

static int32_t calculateControlValue(inputValues* input, controlValues* control);
void PID_setMotor(inputValues* input, controlValues* control, int32_t target, uint8_t side);

/*
 * Public functions
 */

void PID_enable(void){
	pid_isActive = 1;
}

void PID_disable(void){
	pid_isActive = 0;
}

void PID_trigger(void){
	if(!pid_isActive){
		return;
	}
	odo_status stat = odometry_getStatus();
	left_input.inputValue = targetValues.targetValueLeft - stat.left.speed;
	right_input.inputValue = targetValues.targetValueRight - stat.right.speed;
	PID_setMotor(&left_input, &left_control, targetValues.targetValueLeft, MOTORLEFT);
	PID_setMotor(&right_input, &right_control, targetValues.targetValueRight, MOTORRIGHT);
}

void PID_setParameter(int32_t kP, int32_t kI, int32_t kD){
	Kp = kP;
	Ki = kI;
	Kd = kD;
}

void PID_setSpeed(int32_t leftSpeed, int32_t rightSpeed){
	targetValues.targetValueLeft = leftSpeed;
	targetValues.targetValueRight = rightSpeed;
}

/*
 * Private functions
 */

void PID_setMotor(inputValues* input, controlValues* control, int32_t target, uint8_t side){
	control->controlValue = calculateControlValue(input, control);
	control->controlValue = motor_setSpeed(side, control->controlValue);

	control->controlValueOld = control->controlValue;
	control->controlValueOld2 = input->inputValueOld;
	input->inputValueOld = input->inputValue;
}

int32_t calculateControlValue(inputValues* input, controlValues* control){
	q0 = (Kp + (Ki*deltaT) + (Kd/deltaT));
	q1 = -Kp -2*(Kd/deltaT);
	q2 = Kd/deltaT;
	volatile int32_t erg = control->controlValueOld;

	erg += (q0*input->inputValue/100);
	erg += (q1*input->inputValueOld/100);
	erg += (q2*input->inputValueOld2/100);

	return erg;
}
