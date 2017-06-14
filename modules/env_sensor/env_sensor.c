/*
* env_sensor.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "env_sensor.h"

volatile uint32_t shit = 0;
/**
  * @brief
  * @param  None
  * @retval None
  */
void env_timer_init(){

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);

	/* Enable the timer peripheral clock */
	LL_APB1_GRP1_EnableClock(ENV_SENSING_TIMER_CLK_ENABLE);

	/* Set the pre-scaler value to have TIM5 counter clock equal to 10 kHz */
	LL_TIM_SetPrescaler(ENV_SENSING_TIMER, __LL_TIM_CALC_PSC(SystemCoreClock, ENV_SENSING_TIMER_PSC_FREQ));

	/* Set the auto-reload value to have an initial update event frequency of 10 Hz */
	LL_TIM_SetAutoReload(ENV_SENSING_TIMER, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(ENV_SENSING_TIMER), ENV_SENSING_TIMER_AR_FREQ));

	/* Enable the update interrupt */
	LL_TIM_EnableIT_UPDATE(ENV_SENSING_TIMER);

	/* Set and enablle the Interrupt routine for TIM5 */
	NVIC_SetPriority(ENV_SENSING_TIMER_IRQ, ENV_SENSING_TIMER_PRIO);
	NVIC_EnableIRQ(ENV_SENSING_TIMER_IRQ);

	/* Enable counter */
	LL_TIM_EnableCounter(ENV_SENSING_TIMER);

	/* Force update generation */
	LL_TIM_GenerateEvent_UPDATE(ENV_SENSING_TIMER);
}

/**
  * @brief  This function handles the timer5-interrupt
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
	/* Check whether update interrupt is pending */
	if(LL_TIM_IsActiveFlag_UPDATE(ENV_SENSING_TIMER) == 1)
	{
		/* Clear the update interrupt flag*/
		LL_TIM_ClearFlag_UPDATE(ENV_SENSING_TIMER);
	}

	debug_printf("1Hz Timer5 Interrupt :) - %d s\n", shit);
	shit++;
}
