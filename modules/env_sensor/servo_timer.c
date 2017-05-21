/*
 * servo_timer.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "servo__timer.h"

/**
  * @brief  Servo Clock Configuration
  *         The Servo Clock is configured as follows :
  *            GPIO_PIN_Mode	 	          = Alternate Function
  *            Pin Speed 	                  = FREQ_HIGH
  *            Alternate Function             = Alternate Function 3 (TIM8)
  *            Timer after Prescaler          = 2MHz
  * @param  None
  * @retval None
  */
void servo_timer_init(void){

  /* Enable GPIO-CLK for PORTC */
  MOTOR_GPIO_CLK_ENABLEC();

  /* GPIO TIM8_CH2 configuration */
  LL_GPIO_SetPinMode(MOTORSERVO_PORT, MOTORSERVO_PWM_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(MOTORSERVO_PORT, MOTORSERVO_PWM_PIN, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(MOTORSERVO_PORT, MOTORSERVO_PWM_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(MOTORSERVO_PORT, MOTORSERVO_PWM_PIN, LL_GPIO_AF_3);

  /******************************/
  /* Peripheral clocks enabling */
  /******************************/

  /* Enable the timer peripheral clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);


  /* Set the pre-scaler value to have TIM8 counter clock equal to 2 MHz */
  LL_TIM_SetPrescaler(TIM8, __LL_TIM_CALC_PSC(SystemCoreClock, 2000000));

  /* Enable TIM8_ARR register preload. Writing to or reading from the     */
  /* auto-reload register accesses the preload register.			      */
  LL_TIM_EnableARRPreload(TIM8);

  /* Set the auto-reload value to have a counter frequency of 50 Hz */
  LL_TIM_SetAutoReload(TIM8, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM8), 50));

  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  LL_TIM_OC_SetMode(TIM8, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);

  /* Set compare value */
  LL_TIM_OC_SetCompareCH2(TIM8, 1050);


  LL_TIM_OC_EnablePreload(TIM8, LL_TIM_CHANNEL_CH2);

  /**********************************/
  /* Start output signal generation */
  /**********************************/
  /* Enable Output Channel 2 */
  LL_TIM_CC_EnableChannel(TIM8, LL_TIM_CHANNEL_CH2);


  /* Enable counter */
  LL_TIM_EnableCounter(TIM8);

  /* Enable Outputs for TIM8 CH2 */
  LL_TIM_EnableAllOutputs(TIM8);
}
