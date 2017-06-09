/*
 * servo__timer.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef ENV_SENSOR_SERVO_TIMER_H_
#define ENV_SENSOR_SERVO_TIMER_H_

#define MOTORSERVO_PORT					   GPIOC
#define MOTOR_GPIO_CLK_ENABLEC()		   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
#define MOTORSERVO_PWM_PIN				   LL_GPIO_PIN_7


#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

void servo_timer_init(void);

#endif /* ENV_SENSOR_SERVO_TIMER_H_ */
