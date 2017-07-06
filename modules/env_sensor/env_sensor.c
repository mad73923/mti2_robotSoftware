/*
* env_sensor.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "env_sensor.h"

/**
  * @brief  Servo Clock Configuration
  *         The Servo Clock is configured as follows :
  *            Timer befor Prescaler		  	= 80MHz
  *            Timer after Prescaler          	= 10kHz
  *            Timer Interrupt Frequency	  	= 12Hz
  *            Timer Interrupt Priority 		= 3
  * @param  None
  * @retval None
  */
void env_timer_init(){

	/* Enable the timer peripheral clock */
	LL_APB1_GRP1_EnableClock(ENV_SENSING_TIMER_CLK_ENABLE);

	/* Set the pre-scaler value to have ENV_SENSING_TIMER counter clock equal to ENV_SENSING_TIMER_PSC_FREQ kHz */
	LL_TIM_SetPrescaler(ENV_SENSING_TIMER, __LL_TIM_CALC_PSC(SystemCoreClock, ENV_SENSING_TIMER_PSC_FREQ));

	/* Set the auto-reload value to have an initial update event frequency of ENV_SENSING_TIMER_AR_FREQ */
	LL_TIM_SetAutoReload(ENV_SENSING_TIMER, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(ENV_SENSING_TIMER), ENV_SENSING_TIMER_AR_FREQ));

	/* Enable the update interrupt */
	LL_TIM_EnableIT_UPDATE(ENV_SENSING_TIMER);

	/* Set and enable the Interrupt routine for ENV_SENSING_TIMER */
	NVIC_SetPriority(ENV_SENSING_TIMER_IRQ, ENV_SENSING_TIMER_PRIO);
	NVIC_EnableIRQ(ENV_SENSING_TIMER_IRQ);

	/* Enable counter */
	LL_TIM_EnableCounter(ENV_SENSING_TIMER);

	/* Force update generation */
	LL_TIM_GenerateEvent_UPDATE(ENV_SENSING_TIMER);
}

/**
* @brief  This function starts the environment sensing.
* @param  None
* @retval None
*/
void start_env_data_collector()
{
	startEnvFlag=1;
}

/**
* @brief  This function stops the environment sensing
* @param  None
* @retval None
*/
void stop_env_data_collector()
{
	startEnvFlag=0;
}

/**
  * @brief  This function handles the timer5-interrupt
  * 		which is used for starting an adc-conversion.
  *
  * @param  None
  * @retval None
  */
void ENV_SENSING_TIMER_INTERRUPT_HANDLER()
{
	/* Check whether update interrupt is pending */
	if(LL_TIM_IsActiveFlag_UPDATE(ENV_SENSING_TIMER) == 1)
	{
		/* Clear the update interrupt flag*/
		LL_TIM_ClearFlag_UPDATE(ENV_SENSING_TIMER);
	}

	/* Checking whether ADC is still busy converting or the ADC is still not configured */
	if ((LL_ADC_IsEnabled(ADC_NR) == 1) && (LL_ADC_IsDisableOngoing(ADC_NR) == 0) && (LL_ADC_REG_IsConversionOngoing(ADC_NR) == 0) && (startEnvFlag == 1))
	{
		/* Start ADC conversion */
		LL_ADC_REG_StartConversion(ADC_NR);
	}


}
