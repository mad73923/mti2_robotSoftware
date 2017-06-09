/*
 * adc.h
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */

#ifndef ENV_SENSOR_ADC_H_
#define ENV_SENSOR_ADC_H_

#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_bus.h"
#include <math.h>

/*Defines*/
#define ADC_CALIBRATION_TIMEOUT_MS       ((uint32_t)   1)
#define ADC_ENABLE_TIMEOUT_MS            ((uint32_t)   1)
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)   2)

/*Struct distValues*/
struct distValues {
	uint16_t front;
	uint16_t back;
};
struct distValues getFrontBackDistance(void);

/*Public Function prototypes*/
void    Configure_ADC(void);
void    Activate_ADC(void);
uint16_t getFrontSensorValue();
uint16_t getBackSensorValue();

/*Private Function prototypes*/
void 	ADC1_2_IRQHandler(void);

/* Public variables*/
/* Variables for ADC conversion data */
__IO uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /* ADC group regular conversion data */
__IO uint8_t frontFlag; /*Flag to differentiate between front-sensor and back-sensor data*/

#endif /* ENV_SENSOR_ADC_H_ */
