/*
 * env_sensor.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef ENV_SENSOR_ENV_SENSOR_H_
#define ENV_SENSOR_ENV_SENSOR_H_

/*Private Defines*/
#define ENV_SENSING_TIMER_CLK_ENABLE			LL_APB1_GRP1_PERIPH_TIM5
#define ENV_SENSING_TIMER_PRIO 					3
#define ENV_SENSING_TIMER 						TIM5
#define ENV_SENSING_TIMER_IRQ 					TIM5_IRQn
#define ENV_SENSING_TIMER_INTERRUPT_HANDLER()	TIM5_IRQHandler()
#define ENV_SENSING_TIMER_PSC_FREQ				10000
#define ENV_SENSING_TIMER_AR_FREQ				12


#include "adc.h"
#include "../debug/debug.h"

/*Global Functions*/
void env_timer_init();
void start_env_data_collector();
void stop_env_data_collector();

#endif /* ENV_SENSOR_ENV_SENSOR_H_ */
