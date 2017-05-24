/*
 * motor_timer.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "motor_timer.h"

/**
  * @brief  With this function the Timer of both PWM-Channels (MotorLeft, MotorRight)
  * 		will be configured. I dunno the times by now, cuz I'm too lazy...later...
  * @param  None
  * @retval None
  */
void motor_timer_init(void){

	/* Enable the MotorLeft/MotorRight_GPÌO_Clock */
	MOTORRIGHT_GPIO_CLK_ENABLEA();
	MOTORLEFT_GPIO_CLK_ENABLEB();

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

	/* Set the pre-scaler value to have TIM4&3 counter clock equal to 200 kHz */
	LL_TIM_SetPrescaler(TIM4, __LL_TIM_CALC_PSC(SystemCoreClock, 400000));
	LL_TIM_SetPrescaler(TIM3, __LL_TIM_CALC_PSC(SystemCoreClock, 400000));

	/* Enable TIM4|3_ARR register preload. Writing to or reading from the         */
	/* auto-reload register accesses the preload register. The content of the   */
	/* preload register are transferred into the shadow register at each update */
	/* event (UEV).                                                             */
	LL_TIM_EnableARRPreload(TIM4);
	LL_TIM_EnableARRPreload(TIM3);

	/* Set the auto-reload value to have a counter frequency of 200 Hz */
	LL_TIM_SetAutoReload(TIM4, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM4), 200));
	LL_TIM_SetAutoReload(TIM3, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM3), 200));

	/*********************************/
	/* Output waveform configuration */
	/*********************************/
	/* Set output mode */
	/* Reset value is LL_TIM_OCMODE_FROZEN */
	LL_TIM_OC_SetMode(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);

	/* Set compare value to half of the counter period (50% duty cycle ) */
	LL_TIM_OC_SetCompareCH1(TIM4, (LL_TIM_GetAutoReload(TIM4)/2));
	LL_TIM_OC_SetCompareCH2(TIM3, (LL_TIM_GetAutoReload(TIM3)/2));

	/* Enable TIM2_CCR1 register preload. Read/Write operations access the      */
	/* preload register. TIM2_CCR1 preload value is loaded in the active        */
	/* at each update event.                                                    */
	LL_TIM_OC_EnablePreload(TIM4, LL_TIM_CHANNEL_CH1);
	LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH2);

	/**********************************/
	/* Start output signal generation */
	/**********************************/
	/* Enable output channel 1/2 */
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH2);

	/* Enable counter for timer4/3 */
	LL_TIM_EnableCounter(TIM4);
	LL_TIM_EnableCounter(TIM3);
}

/**
  * @brief  With this function the a specific Motor will be controlled by direction and speed.
  * @param  cMotor (Left/Right Motor) cDir(Forward, Backward) iCC(CaptureCompare (0-100))
  * @retval None
  */
void set_cc(char cMotor,char cDir, int iCC){


}
