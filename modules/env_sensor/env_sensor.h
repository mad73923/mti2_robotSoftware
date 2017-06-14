/*
 * env_sensor.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef ENV_SENSOR_ENV_SENSOR_H_
#define ENV_SENSOR_ENV_SENSOR_H_

/*Private Defines*/
#define NR_VALUES 36

#include "servo_timer.h"
#include "adc.h"

/*Global Variables*/
uint16_t distances_data [NR_VALUES];

/*Global Functions*/
void env_timer_init();
uint16_t start_env_data_collector();
uint16_t stop_env_data_collector();

#endif /* ENV_SENSOR_ENV_SENSOR_H_ */
