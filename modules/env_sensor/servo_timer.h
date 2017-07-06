/*
 * servo__timer.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef ENV_SENSOR_SERVO_TIMER_H_
#define ENV_SENSOR_SERVO_TIMER_H_

#define MOTORSERVO_PORT					   	GPIOC
#define MOTORSERVO_PWM_PIN				   	LL_GPIO_PIN_7
#define MOTORSERVO_PWM_CHANNEL				LL_TIM_CHANNEL_CH2
#define MOTORSERVO_TIMER					TIM8
#define MOTORSERVO_TIMER_PSC_FREQ			2000000
#define MOTORSERVO_TIMER_AR_FREQ			50
#define MOTORSERVO_PWM_CC_VALUE				1050
#define MOTOR_GPIO_CLK_ENABLE()			   	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
#define TIMER_CLK_ENABLE				  	LL_APB2_GRP1_PERIPH_TIM8

#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

void servo_timer_init(void);

#endif /* ENV_SENSOR_SERVO_TIMER_H_ */
