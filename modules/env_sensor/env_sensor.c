/*
* env_sensor.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "env_sensor.h"

/**
  * @brief  This function handles external lines 10 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void env_timer_init(){

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);

	/* Enable the timer peripheral clock */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);

	/* Set the pre-scaler value to have TIM8 counter clock equal to 10 kHz */
	LL_TIM_SetPrescaler(TIM5, __LL_TIM_CALC_PSC(SystemCoreClock, 10000));

	/* Set the auto-reload value to have an initial update event frequency of 10 Hz */
	LL_TIM_SetAutoReload(TIM5, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM5), 1));

	/* Enable the update interrupt */
	LL_TIM_EnableIT_TRIG(TIM5);

	/* Set and enablle the Interrupt routine for timer5 */
	NVIC_SetPriority(TIM5_IRQn, 3);
	NVIC_EnableIRQ(TIM5_IRQn);

	/* Enable counter */
	LL_TIM_EnableCounter(TIM5);

	/* Force update generation */
	LL_TIM_GenerateEvent_TRIG(TIM5);
}

/**
  * @brief  This function handles the timer5-interrupt
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
	LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
}
