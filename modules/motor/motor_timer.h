/*
 * motor_timer.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef MOTOR_MOTOR_TIMER_H_
#define MOTOR_MOTOR_TIMER_H_

#define MOTORLEFT_DIR_PORT                 GPIOA
#define MOTORRIGHT_DIR_PORT				   GPIOA
#define MOTORLEFT_PWM_PORT				   GPIOB
#define MOTORRIGHT_PWM_PORT				   GPIOA
#define PIEP_PORT						   GPIOB
#define MOTORRIGHT_GPIO_CLK_ENABLEA()      LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define MOTORLEFT_GPIO_CLK_ENABLEB()	   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#define MOTORLEFT_DIR_PIN				   LL_GPIO_PIN_6
#define MOTORRIGHT_DIR_PIN				   LL_GPIO_PIN_5
#define MOTORLEFT_PWM_PIN				   LL_GPIO_PIN_6
#define MOTORRIGHT_PWM_PIN				   LL_GPIO_PIN_7
#define PIEP_PIN						   LL_GPIO_PIN_5
#define MOTORLEFT						   0
#define MOTORRIGHT						   1
#define FORWARD							   0
#define BACKWARD					       1
#define NONE							   2
#define SPEED_NONE						   2001

#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

void motor_timer_init(void);
int set_cc(char cMotor, int iCC);
int set_dir(char cMotor, char cDir);
//
#endif /* MOTOR_MOTOR_TIMER_H_ */
