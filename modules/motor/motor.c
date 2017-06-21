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
  * @brief  With this function the speed & direction of a motor will be set.
  * @param  which motor cMotor (MOTOR_LEFT/MOTOR_RIGHT), iSpeed(Speed from -1999-400 (Backwards) & +400-1999 (Forward) @9V Motor Power Supply)
  * @retval The CC-Value of the PWM, if anything went wrong -1
  */
int32_t motor_setSpeed(char cMotor, int32_t iSpeed){

	//Check direction
	if(iSpeed<0){
		set_dir(cMotor,BACKWARD);
		iSpeed*=-1;
	}else{
		set_dir(cMotor,FORWARD);
	}

	//Check Speed-Limit
	if(iSpeed>LL_TIM_GetAutoReload(TIM4)){
		return set_cc(cMotor, LL_TIM_GetAutoReload(TIM4));
	}else{
		return set_cc(cMotor, iSpeed);
	}
}

/**
  * @brief  With this function the motor stops
  * @param  which motor cMotor (MOTOR_LEFT/MOTOR_RIGHT)
  * @retval The CC-Value of the PWM, if anything went wrong -1
  */
void motor_stop(char cMotor){
	set_cc(cMotor,0);
}