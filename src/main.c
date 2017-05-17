/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"

int main(void){
	system_init();
	debug_init();

	odometry_init();

	volatile odo_angles ang;
	while(1){
		ang = odometry_getAngles();
		debug_printf("angle: %f\n", ang.angleWheel_l);
		LL_mDelay(500);
	}
}
