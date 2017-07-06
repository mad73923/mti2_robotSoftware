/*
 * servo.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "servo.h"


/**
  * @brief  With this Function you can set the Angle
  * 		of the Servo_Motor in 10° steps.
  * 		For 0° iAngle has to be 0 and for 180° iAngle
  * 		has to be 18.
  * @param  int iAngle
  * @retval The Compare_Value from the PWM-Channel if the input value is correct
  * 		If the input value is false, then -1 will be returned
  */
uint16_t servo_set_angle(uint16_t iAngle){
	if(iAngle < 0){
		LL_TIM_OC_SetCompareCH2(TIM8, servo_set_angle(0));
		return -1;
	}else if(iAngle > 18){
		LL_TIM_OC_SetCompareCH2(TIM8, servo_set_angle(18));
		return -1;
	}
	LL_TIM_OC_SetCompareCH2(TIM8, sServoLUT[iAngle]);
	return sServoLUT[iAngle];
}
