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
  * @brief  With this function the speed of the
  * @param  cMotor (Which Motor), iSpeed(Speed from 0-1999)
  * @retval None
  *///
int setSpeed(char cMotor, int iSpeed){
	if((iSpeed<0) || (iSpeed>1999)){
		return -1;
	}
	return set_cc(cMotor, NONE, iSpeed);
}


int setDirection(char cMotor,char cDir){
	return set_cc(cMotor,cDir,SPEED_NONE);
}


int setSpeedDir(char cMotor, char cDir, int iSpeed){
	if((iSpeed<0) || (iSpeed>100)){
			return -1;
	}
	return set_cc(cMotor,cDir,iSpeed);
}


int stop(char cMotor){
	return set_cc(cMotor,NONE,0);
}
