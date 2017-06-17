/*
 * adc.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "adc.h"

/*Private Variables*/
__IO float 	linearization = 0.0;
__IO float	a = -1.3838;
__IO float	b = 2.8372;
__IO char arrayIterator = 0;
__IO char arrayFlag = 0;
__IO int8_t counterTwo = 0;

/**
  * @brief  This function configures the ADC1 for PORT A PIN 0 (FRONTSENSOR) & 1 (BACKSENSOR)
  * 		A Interrupt is set and both ADC-Channels are converted
  * 		after another. So two interrupts are needed for a front
  * 		and back sensor data.
  * @param  None
  * @retval None
  */
void adc_init(void)
{
	/* Enable GPIO Clock */
	LL_AHB2_GRP1_EnableClock(PORT_CLK_ENABLE);

	/* Configure GPIO in analog mode to be used as ADC input */
	LL_GPIO_SetPinMode(ADC_PORT, ADC_PIN1, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(ADC_PORT, ADC_PIN2, LL_GPIO_MODE_ANALOG);

	/* Connect GPIO analog switch to ADC input */
	LL_GPIO_EnablePinAnalogControl(ADC_PORT, ADC_PIN1);
	LL_GPIO_EnablePinAnalogControl(ADC_PORT, ADC_PIN2);

	/* Configure NVIC to enable ADC1 interruptions */
	NVIC_SetPriority(ADC_INTERRUPT, 0);
	NVIC_EnableIRQ(ADC_INTERRUPT);

	/* Enable ADC clock (core clock) */
	LL_AHB2_GRP1_EnableClock(ADC_CLK_ENABLE);

	if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
	{
		LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC_NR), LL_ADC_CLOCK_SYNC_PCLK_DIV2);
	}


	if ((LL_ADC_IsEnabled(ADC_NR) == 0) || (LL_ADC_REG_IsConversionOngoing(ADC_NR) == 0))
	{
		/* Set ADC group regular trigger source */
		LL_ADC_REG_SetTriggerSource(ADC_NR, LL_ADC_REG_TRIG_SOFTWARE);

		/* Set ADC group regular continuous mode */
		LL_ADC_REG_SetContinuousMode(ADC_NR, LL_ADC_REG_CONV_SINGLE);

		/* Set ADC group regular overrun behavior */
		LL_ADC_REG_SetOverrun(ADC_NR, LL_ADC_REG_OVR_DATA_OVERWRITTEN);

		/* Set ADC group regular sequencer length and scan direction */
		LL_ADC_REG_SetSequencerLength(ADC_NR, LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS);

		/* Set ADC group regular sequence: channel on the selected sequence rank. */
		LL_ADC_REG_SetSequencerRanks(ADC_NR, LL_ADC_REG_RANK_1, ADC_CHANNEL_1);
		LL_ADC_REG_SetSequencerRanks(ADC_NR, LL_ADC_REG_RANK_2, ADC_CHANNEL_2);
	}

	if ((LL_ADC_IsEnabled(ADC_NR) == 0) || ((LL_ADC_REG_IsConversionOngoing(ADC_NR) == 0) && (LL_ADC_INJ_IsConversionOngoing(ADC_NR) == 0)))
	{
		LL_ADC_SetChannelSamplingTime(ADC_NR, ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_47CYCLES_5);
		LL_ADC_SetChannelSamplingTime(ADC_NR, ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_47CYCLES_5);
	}

	/* Enable interruption ADC group regular end of unitary conversion */
	LL_ADC_EnableIT_EOC(ADC_NR);

	/* Active ADC*/
	__IO uint32_t wait_loop_index = 0;

	if (LL_ADC_IsEnabled(ADC_NR) == 0)
	{
		/* Disable ADC deep power down (enabled by default after reset state) */
		LL_ADC_DisableDeepPowerDown(ADC_NR);

		/* Enable ADC internal voltage regulator */
		LL_ADC_EnableInternalRegulator(ADC_NR);

		wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);

		while(wait_loop_index != 0)
		{
		  wait_loop_index--;
		}

		/* Run ADC self calibration */
		LL_ADC_StartCalibration(ADC_NR, LL_ADC_SINGLE_ENDED);

		while ((LL_ADC_IsCalibrationOnGoing(ADC_NR) != 0)){}

		/* Delay between ADC end of calibration and ADC enable.                   */
		/* Note: Variable divided by 2 to compensate partially                    */
		/*       CPU processing cycles (depends on compilation optimization).     */
		wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
		while(wait_loop_index != 0)
		{
		  wait_loop_index--;
		}

		/* Enable ADC */
		LL_ADC_Enable(ADC_NR);
		/* Poll for ADC ready to convert */
		while ((LL_ADC_IsActiveFlag_ADRDY(ADC_NR) == 0)){}
	}
}

/**
  * @brief  This function handles ADC1 interrupt request.
  * 		For both sensors it needs two interrupts to get
  * 		both values (Rank0, 1). They are stored in the array
  * 		aADCxConvertedData[]. A flag is switched to ensure
  * 		that the data is on the right place.
  * @param  None
  * @retval None
  */
void ADC_INTERRUPT_HANDLER()
{
	/*Clear ADC EOC Flag*/
	LL_ADC_ClearFlag_EOC(ADC_NR);

	/* ADC on Pin PA0 is first converted, then PA1. The very first converted value corresponds to PA0 */
	/* With this formula, the internal ADC-value is converted into mm-distance */
	linearization = (10*exp(b)* pow((LL_ADC_REG_ReadConversionData12(ADC_NR)/1241.21),a));

	/* This function block writes the linearized ADC-Distance-Value into the distances-array.
	 * To start this function block the corresponding function in env_sensor.c has to be called.
	 * The frontFlag is to differentiate between front and back-sensor-data. */
	if(startEnvFlag){

		if(frontFlag){
			distances_data[arrayIterator]=(uint16_t)linearization;
		}else{
			distances_data[arrayIterator+PI_OFFSET]=(uint16_t)linearization;
		}

		if(counterTwo == 1){

			if(arrayFlag){
				servo_set_angle(arrayIterator--);
			}else{
				servo_set_angle(arrayIterator++);
			}

			counterTwo=-1;
		}
		counterTwo++;

		if(arrayIterator == 19){
			arrayFlag = !arrayFlag;
			arrayIterator = 17;
		}else if(arrayIterator == 255){
			arrayFlag = !arrayFlag;
			arrayIterator=1;
		}

	}

	aADCxConvertedData[frontFlag] = linearization;
	frontFlag = !frontFlag;
}

/**
  * @brief  This function maps the distance front and back values
  * 		into a distValues struct and returns it.
  * @param  None
  * @retval Struct with distance values front and back in mm
  */
struct distValues getFrontBackDistance(void)
{
	struct distValues distances;
	distances.front = aADCxConvertedData[0];
	distances.back = aADCxConvertedData[1];
	return distances;
}

/**
  * @brief  This function returns the front sensor value.
  * @param  None
  * @retval Front value in mm
  */
uint16_t getFrontSensorValue()
{
	return aADCxConvertedData[0];
}

/**
  * @brief  This function returns the back sensor value.
  * @param  None
  * @retval Back value in mm
  */
uint16_t getBackSensorValue()
{
	return aADCxConvertedData[1];
}

/**
  * @brief  This function returns the pointer of the first distances-array-value.
  * @param  None
  * @retval Back value in mm
  */
uint16_t * getDistancesData()
{
	return distances_data;
}

/**
  * @brief  This function returns the current index of the written distance-sensor
  * 		array position.
  * @param  None
  * @retval Back value in mm
  */
char getCurrentDistanceArrayIndex()
{
	return arrayIterator;
}
