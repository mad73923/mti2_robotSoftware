/*
 * motor_timer.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "motor_timer.h"

/**
  * @brief  With this function the Timer of both PWM-Channels (MotorLeft, MotorRight)
  * 		will be configured.
  * @param  None
  * @retval None
  *///
void motor_timer_init(void){

	/* Enable the MotorLeft/MotorRight_GPÌO_Clock */
	MOTORRIGHT_GPIO_CLK_ENABLEA();
	MOTORLEFT_GPIO_CLK_ENABLEB();

	/* Enable Pin-Outputs */
	configure_gpio();

	/* GPIO TIM4_CH1 configuration */
	LL_GPIO_SetPinMode(MOTORLEFT_PWM_PORT, MOTORLEFT_PWM_PIN, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(MOTORLEFT_PWM_PORT, MOTORLEFT_PWM_PIN, LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinSpeed(MOTORLEFT_PWM_PORT, MOTORLEFT_PWM_PIN, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetAFPin_0_7(MOTORLEFT_PWM_PORT, MOTORLEFT_PWM_PIN, LL_GPIO_AF_2);
	/* GPIO TIM3_CH2 configuration*/
	LL_GPIO_SetPinMode(MOTORRIGHT_PWM_PORT, MOTORRIGHT_PWM_PIN, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(MOTORRIGHT_PWM_PORT, MOTORRIGHT_PWM_PIN, LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinSpeed(MOTORRIGHT_PWM_PORT, MOTORRIGHT_PWM_PIN, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetAFPin_0_7(MOTORRIGHT_PWM_PORT, MOTORRIGHT_PWM_PIN, LL_GPIO_AF_2);

	/******************************/
	/* Peripheral clocks enabling */
	/******************************/
	/* Enable the timer peripheral clock */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

	/* Set the pre-scaler value to have TIM4&3 counter clock equal to 400 kHz */
	LL_TIM_SetPrescaler(MOTORLEFT_TIMER, __LL_TIM_CALC_PSC(SystemCoreClock, MOTORLEFT_PRESCALER_FREQ));
	LL_TIM_SetPrescaler(MOTORRIGHT_TIMER, __LL_TIM_CALC_PSC(SystemCoreClock, MOTORRIGHT_PRESCALER_FREQ));

	/* Enable TIM4|3_ARR register preload. Writing to or reading from the         */
	/* auto-reload register accesses the preload register. The content of the   */
	/* preload register are transferred into the shadow register at each update */
	/* event (UEV).                                                             */
	LL_TIM_EnableARRPreload(MOTORLEFT_TIMER);
	LL_TIM_EnableARRPreload(MOTORRIGHT_TIMER);

	/* Set the auto-reload value to have a counter frequency of 200 Hz */
	LL_TIM_SetAutoReload(MOTORLEFT_TIMER, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(MOTORLEFT_TIMER), MOTORLEFT_ARR_FREQ));
	LL_TIM_SetAutoReload(MOTORRIGHT_TIMER, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(MOTORRIGHT_TIMER), MOTORRIGHT_ARR_FREQ));

	/*********************************/
	/* Output waveform configuration */
	/*********************************/
	/* Set output mode */
	/* Reset value is LL_TIM_OCMODE_FROZEN */
	LL_TIM_OC_SetMode(MOTORLEFT_TIMER, MOTORLEFT_TIMER_PWM_CHANNEL, LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetMode(MOTORRIGHT_TIMER, MOTORRIGHT_TIMER_PWM_CHANNEL, LL_TIM_OCMODE_PWM1);

	/* Set compare value to half of the counter period (50% duty cycle ) */
	LL_TIM_OC_SetCompareCH1(MOTORLEFT_TIMER, 0);
	LL_TIM_OC_SetCompareCH2(MOTORRIGHT_TIMER, 0);

	/* Enable TIM2_CCR1 register preload. Read/Write operations access the      */
	/* preload register. TIM2_CCR1 preload value is loaded in the active        */
	/* at each update event.                                                    */
	LL_TIM_OC_EnablePreload(MOTORLEFT_TIMER, MOTORLEFT_TIMER_PWM_CHANNEL);
	LL_TIM_OC_EnablePreload(MOTORRIGHT_TIMER, MOTORRIGHT_TIMER_PWM_CHANNEL);

	/**********************************/
	/* Start output signal generation */
	/**********************************/
	/* Enable output channel 1/2 */
	LL_TIM_CC_EnableChannel(MOTORLEFT_TIMER, MOTORLEFT_TIMER_PWM_CHANNEL);
	LL_TIM_CC_EnableChannel(MOTORRIGHT_TIMER, MOTORRIGHT_TIMER_PWM_CHANNEL);

	/* Enable counter for timer4/3 */
	LL_TIM_EnableCounter(MOTORLEFT_TIMER);
	LL_TIM_EnableCounter(MOTORRIGHT_TIMER);
}

/**
  * @brief  With this function the speed of a specific Motor will be set.
  * @param  cMotor (Left/Right Motor), iCC(CaptureCompare (0-100))
  * @retval None
  */
int32_t set_cc(char cMotor, int32_t iCC){

	if(cMotor == MOTORLEFT){
		LL_TIM_OC_SetCompareCH2(MOTORRIGHT_TIMER, iCC);
		return iCC;
	}else if(cMotor == MOTORRIGHT){
		LL_TIM_OC_SetCompareCH1(MOTORLEFT_TIMER, iCC);
		return iCC;
	}else{
		return -1;
	}
}

/**
  * @brief  With this function the direction of a specific Motor will be set.
  * @param  cMotor (Left/Right Motor) cDir(Forward, Backward) iCC(CaptureCompare (0-100))
  * @retval None
  */
int32_t set_dir(char cMotor,char cDir){

	if(cMotor == MOTORLEFT){
		if(cDir==FORWARD){
			LL_GPIO_SetOutputPin(MOTORLEFT_DIR_PORT, MOTORLEFT_DIR_PIN);
			return FORWARD;
		}else{
			LL_GPIO_ResetOutputPin(MOTORLEFT_DIR_PORT, MOTORLEFT_DIR_PIN);
			return BACKWARD;
		}
	}

	if(cMotor == MOTORRIGHT){
		if(cDir==FORWARD){
			LL_GPIO_SetOutputPin(MOTORRIGHT_DIR_PORT, MOTORRIGHT_DIR_PIN);
			return FORWARD;
		}else{
			LL_GPIO_ResetOutputPin(MOTORRIGHT_DIR_PORT, MOTORRIGHT_DIR_PIN);
			return BACKWARD;
		}
	}
	return -1; //Failure
}
