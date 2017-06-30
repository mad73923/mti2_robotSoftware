/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"


/*MOTOR-DRIVER-TEST*/
int main(void){
	system_init();

	motor_timer_init();

	while(1){

		motor_setSpeed(MOTORRIGHT,700);
		motor_setSpeed(MOTORLEFT,700);
		LL_mDelay(2000);
		motor_setSpeed(MOTORRIGHT,-700);
		motor_setSpeed(MOTORLEFT,-700);
		LL_mDelay(2000);
		motor_setSpeed(MOTORRIGHT,700);
		motor_setSpeed(MOTORLEFT,700);
		LL_mDelay(2000);
		motor_stop(MOTORLEFT);
		motor_stop(MOTORRIGHT);
		LL_mDelay(2000);
	}
}
