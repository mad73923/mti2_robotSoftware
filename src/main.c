/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"

<<<<<<< HEAD

/*MOTOR-DRIVER-TEST*/
||||||| merged common ancestors
=======
static volatile odo_status stat;

>>>>>>> master
int main(void){
	system_init();

	odometry_init();
	motor_timer_init();

	while(1){
		odometry_updateStatus_async();
	}
}
