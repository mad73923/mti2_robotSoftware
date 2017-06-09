/*
 * adc.c
 *
 *  Created on: 17.05.2017
 *      Author: felix
 */
#include "adc.h"

/*Private Variables*/
volatile float 	linearization = 0.0;
volatile float	a = -1.3838;
volatile float	b = 2.8372;

/**
  * @brief  This function configures the ADC1 for PORT A PIN 0 (FRONTSENSOR) & 1 (BACKSENSOR)
  * 		A Interrupt is set and both ADC-Channels are converted
  * 		after another. So two interrupts are needed for a front
  * 		and back sensor data.
  * @param  None
  * @retval None
  */
void Configure_ADC(void)
{
	/* Enable GPIO Clock */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

	/* Configure GPIO in analog mode to be used as ADC input */
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);

	/* Connect GPIO analog switch to ADC input */
	LL_GPIO_EnablePinAnalogControl(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_EnablePinAnalogControl(GPIOA, LL_GPIO_PIN_1);

	/* Configure NVIC to enable ADC1 interruptions */
	NVIC_SetPriority(ADC1_2_IRQn, 0);
	NVIC_EnableIRQ(ADC1_2_IRQn);

	/* Enable ADC clock (core clock) */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);

	if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
	{
		LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_SYNC_PCLK_DIV2);
	}


	if ((LL_ADC_IsEnabled(ADC1) == 0) || (LL_ADC_REG_IsConversionOngoing(ADC1) == 0))
	{
		/* Set ADC group regular trigger source */
		LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);

		/* Set ADC group regular continuous mode */
		LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);

		/* Set ADC group regular overrun behavior */
		LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);

		/* Set ADC group regular sequencer length and scan direction */
		LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS);

		/* Set ADC group regular sequence: channel on the selected sequence rank. */
		LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_5);
		LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_6);
	}

	if ((LL_ADC_IsEnabled(ADC1) == 0) || ((LL_ADC_REG_IsConversionOngoing(ADC1) == 0) && (LL_ADC_INJ_IsConversionOngoing(ADC1) == 0)))
	{
		LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_47CYCLES_5);
		LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_6, LL_ADC_SAMPLINGTIME_47CYCLES_5);
	}

	/* Enable interruption ADC group regular end of unitary conversion */
	LL_ADC_EnableIT_EOC(ADC1);

}

/**
  * @brief  This function enables the ADC1.
  * @param  None
  * @retval None
  */
void Activate_ADC(void)
{
  __IO uint32_t wait_loop_index = 0;

  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Disable ADC deep power down (enabled by default after reset state) */
    LL_ADC_DisableDeepPowerDown(ADC1);

    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(ADC1);

    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);

    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    /* Run ADC self calibration */
    LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);

    while ((LL_ADC_IsCalibrationOnGoing(ADC1) != 0)){}

    /* Delay between ADC end of calibration and ADC enable.                   */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    /* Enable ADC */
    LL_ADC_Enable(ADC1);
    /* Poll for ADC ready to convert */
    while ((LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)){}
  }
}


/**
  * @brief  This function handles ADC1 interrupt request.
  * 		For both sensors it needs two interrupts to get
  * 		both values. They are stored in the array
  * 		aADCxConvertedData[]. A flag is switched to ensure
  * 		that the data is on the right place.
  * @param  None
  * @retval None
  */
void ADC1_2_IRQHandler(void)
{
	/*Clear ADC EOC Flag*/
	LL_ADC_ClearFlag_EOC(ADC1);

	//ADC on Pin PA0 is first converted, then PA1. The very first converted value corresponds to PA0!!!
	linearization = (exp(b)* pow((LL_ADC_REG_ReadConversionData12(ADC1)/1241.21),a))*10;
	aADCxConvertedData[frontFlag] = (uint16_t)linearization;
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
