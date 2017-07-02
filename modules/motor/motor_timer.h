/*
 * motor_timer.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef MOTOR_MOTOR_TIMER_H_
#define MOTOR_MOTOR_TIMER_H_

#define MOTORLEFT_TIMER						TIM4
#define MOTORRIGHT_TIMER					TIM3
#define MOTORLEFT_TIMER_PWM_CHANNEL			LL_TIM_CHANNEL_CH1
#define MOTORRIGHT_TIMER_PWM_CHANNEL		LL_TIM_CHANNEL_CH2
#define MOTORLEFT_DIR_PORT                 	GPIOA
#define MOTORRIGHT_DIR_PORT				   	GPIOA
#define MOTORLEFT_PWM_PORT				   	GPIOB
#define MOTORRIGHT_PWM_PORT				   	GPIOA
#define PIEP_PORT						   	GPIOB
#define MOTORRIGHT_GPIO_CLK_ENABLEA()      	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define MOTORLEFT_GPIO_CLK_ENABLEB()	   	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#define MOTORLEFT_PRESCALER_FREQ			400000
#define MOTORRIGHT_PRESCALER_FREQ			400000
#define MOTORLEFT_ARR_FREQ					200
#define MOTORRIGHT_ARR_FREQ					200
#define MOTORLEFT_DIR_PIN				   	LL_GPIO_PIN_5
#define MOTORRIGHT_DIR_PIN				   	LL_GPIO_PIN_6
#define MOTORLEFT_PWM_PIN				   	LL_GPIO_PIN_6
#define MOTORRIGHT_PWM_PIN				   	LL_GPIO_PIN_7
#define PIEP_PIN						   	LL_GPIO_PIN_5
#define MOTORLEFT						   	0
#define MOTORRIGHT						   	1
#define FORWARD							   	0
#define BACKWARD					       	1

#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

void motor_timer_init(void);
int32_t set_cc(char cMotor, int32_t iCC);
int32_t set_dir(char cMotor, char cDir);
//
#endif /* MOTOR_MOTOR_TIMER_H_ */
