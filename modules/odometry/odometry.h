/*
 * odometry.h
 *
 *  Created on: 11.05.2017
 *      Author: matthias
 */

#ifndef ODOMETRY_ODOMETRY_H_
#define ODOMETRY_ODOMETRY_H_

#include "TLE5012B.h"
#include "stm32l4xx_ll_tim.h"
#include <math.h>

typedef struct{
	float angle;
	int16_t revolutions;
}wheel;

typedef struct{
	float posX;
	float posY;
}point;

typedef struct{
	wheel left;
	wheel right;
	point position;
	float theta;
}odo_status;

// Hardware defines
#define ODO_TIM_CLK_INIT() LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6)
#define ODO_TIM_INST TIM6
#define ODO_TIM_IRQN TIM6_DAC_IRQn
#define ODO_TIM_PRIO 10
#define ODO_TIM_FREQ 1
#define ODO_TIM_IRQ_HANDLER() TIM6_DAC_IRQHandler()

// all measures in mm
#define ODO_wheelRadius 32.5
#define ODO_halfAxialLength 69.0

void odometry_init(void);
void odometry_updateStatus_async(void);
odo_status odometry_getStatus(void);
void odometry_setStatus(float x, float y, float theta);

#endif /* ODOMETRY_ODOMETRY_H_ */
