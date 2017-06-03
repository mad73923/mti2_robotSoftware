/*
 * motor.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "motor.h"

/**
  * @brief  With this function the GPIO for the direction for MotorLeft &
  * MotorRight will be configured as Output. Furthermore the Beep Output-Pin
  * will be configured.
  * @param  None
  * @retval None
  *///
void configure_gpio(void){
  /* Configure IO in output push-pull mode to drive external LED2 & Motor*/
  LL_GPIO_SetPinMode(MOTORLEFT_DIR_PORT, MOTORLEFT_DIR_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(MOTORRIGHT_DIR_PORT, MOTORRIGHT_DIR_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(PIEP_PORT, PIEP_PIN, LL_GPIO_MODE_OUTPUT);

  /* Predefine Motor-Directions */
  LL_GPIO_SetOutputPin(MOTORLEFT_DIR_PORT, MOTORLEFT_DIR_PIN);
  LL_GPIO_SetOutputPin(MOTORRIGHT_DIR_PORT, MOTORRIGHT_DIR_PIN);
}

/**
  * @brief  With this function the speed of a motor will be set.
  * @param  which motor cMotor (MOTOR_LEFT/MOTOR_RIGHT), iSpeed(Speed from 400-1999 @9V Motor Power Supply)
  * @retval The CC-Value of the PWM, if anything went wrong -1
  */
int setSpeed(char cMotor, int iSpeed){
	if((iSpeed<400) || (iSpeed>1999)){
		return -1;
	}
	return set_cc(cMotor, iSpeed);
}

/**
  * @brief  With this function the direction of a motor will be changed
  * @param  which motor cMotor (MOTOR_LEFT/MOTOR_RIGHT), the direction cDir (FORWARD/BACKWARD)
  * @retval The direction value
  */
int setDirection(char cMotor,char cDir){
	return set_dir(cMotor,cDir);
}

/**
  * @brief  With this function the speed and direction of a motor will be changed
  * @param  which motor cMotor (MOTOR_LEFT/MOTOR_RIGHT), the direction cDir (FORWARD/BACKWARD),  iSpeed(Speed from 400-1999 @9V Motor Power Supply)
  * @retval The CC-Value of the PWM, if anything went wrong -1
  */
int setSpeedDir(char cMotor, char cDir, int iSpeed){
	if((iSpeed<0) || (iSpeed>100)){
			return -1;
	}
	set_dir(cMotor,cDir);
	return set_cc(cMotor,iSpeed);
}

/**
  * @brief  With this function the motor stops
  * @param  which motor cMotor (MOTOR_LEFT/MOTOR_RIGHT)
  * @retval The CC-Value of the PWM, if anything went wrong -1
  */
int stop(char cMotor){
	return set_cc(cMotor,0);
}