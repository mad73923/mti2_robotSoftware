/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"

#define LED2_PIN                           LL_GPIO_PIN_5
#define LED2_GPIO_PORT                     GPIOA
#define LED2_GPIO_CLK_ENABLE()             LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)


/*!!!SERVO TEST!!!*/
int main(void){
	system_init();

	//Init
	env_timer_init();
	servo_timer_init();
	int i = 0;
	servo_set_angle(i);

	while(1){

		LL_mDelay(200);
		i++;
		if(i==19){
			i=0;
			servo_set_angle(i);
			LL_mDelay(400);
		}
		servo_set_angle(i);

	}
}


/*ADC-DRIVER_TEST*/
//int main(void){
//	system_init();
//
//	//Init
//	LED2_GPIO_CLK_ENABLE();
//	LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
//	Configure_ADC();
//	Activate_ADC();
//	uint32_t ii=0;
//	int32_t shit=0;
//
//	while(1){
//
//		ii++;
//		if(ii==100000){
//			if ((LL_ADC_IsEnabled(ADC1) == 1) && (LL_ADC_IsDisableOngoing(ADC1) == 0) && (LL_ADC_REG_IsConversionOngoing(ADC1) == 0))
//			{
//			  LL_ADC_REG_StartConversion(ADC1);
//			}
//			ii=0;
//			shit = getBackSensorValue();
//		}
//		if(shit < 200){
//			LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
//		}else{
//			LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
//		}
//
//	}
//}


/*MOTOR-DRIVER-TEST*/
//int main(void){
//	system_init();
//
//	motor_timer_init();
//
//	while(1){
//
//		motor_setSpeed(MOTORRIGHT,700);
//		motor_setSpeed(MOTORLEFT,700);
//		LL_mDelay(2000);
//		motor_setSpeed(MOTORRIGHT,-700);
//		motor_setSpeed(MOTORLEFT,-700);
//		LL_mDelay(2000);
//		motor_setSpeed(MOTORRIGHT,700);
//		motor_setSpeed(MOTORLEFT,700);
//		LL_mDelay(2000);
//		motor_stop(MOTORLEFT);
//		motor_stop(MOTORRIGHT);
//		LL_mDelay(2000);
//	}
//}
