/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"


int main(void){
	system_init();

	//odometry_init();
	motor_timer_init();
	motor_setSpeed(MOTORLEFT, 0);
	motor_setSpeed(MOTORRIGHT, 0);

	while(1){
		//odometry_updateStatus_async();
		motor_setSpeed(MOTORLEFT, -500);
		LL_mDelay(2000);
		motor_setSpeed(MOTORLEFT, 700);
		LL_mDelay(2000);
	}
}
