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
#include "servo_timer.h"
#include <math.h>

/* Defines */
#define ADC_CALIBRATION_TIMEOUT_MS       	((uint32_t)   1)
#define ADC_ENABLE_TIMEOUT_MS            	((uint32_t)   1)
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  	(LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define ADC_CONVERTED_DATA_BUFFER_SIZE   	((uint32_t)   2)
#define ADC_NR							 	ADC1
#define ADC_CHANNEL_1						LL_ADC_CHANNEL_5
#define ADC_CHANNEL_2						LL_ADC_CHANNEL_6
#define ADC_CLK_ENABLE						LL_AHB2_GRP1_PERIPH_ADC
#define ADC_PORT							GPIOA
#define ADC_PIN1							LL_GPIO_PIN_0
#define ADC_PIN2							LL_GPIO_PIN_1
#define ADC_INTERRUPT						ADC1_2_IRQn
#define ADC_INTERRUPT_HANDLER()				ADC1_2_IRQHandler()
#define ADC_PRIORITY						14
#define PORT_CLK_ENABLE						LL_AHB2_GRP1_PERIPH_GPIOA
#define NR_VALUES 36
#define PI_OFFSET 18
#define MAX_DIST  1000
/* Struct distValues in mm */
struct distValues {
	uint16_t front;
	uint16_t back;
};
struct distValues getFrontBackDistance(void);

/* Public Function prototypes */
void    adc_init(void);
uint16_t getFrontSensorValue();
uint16_t getBackSensorValue();
uint16_t * getDistancesArray();
uint16_t * getDistancesArrayShadow();
char getCurrentDistanceArrayIndex();
void linearizeADCRawData();
void setMutexShadow();
void resetMutexShadow();
uint8_t getEnvFlag();


/* Private Function prototypes */
void 	ADC1_2_IRQHandler(void);

/* Public variables */
/* Variables for ADC conversion data */
__IO uint16_t analogRawData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /* ADC group regular conversion data in mm */
__IO uint16_t analoglinearizedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /* ADC group regular conversion data in mm */
__IO uint16_t distances_data [NR_VALUES];
__IO uint16_t distances_shadow [NR_VALUES];
__IO uint8_t frontFlag; /* Flag to differentiate between front-sensor and back-sensor data */
__IO uint8_t startEnvFlag; /* Flat to start environment sensing */
__IO uint8_t mutexShadow;

#endif /* ENV_SENSOR_ADC_H_ */
