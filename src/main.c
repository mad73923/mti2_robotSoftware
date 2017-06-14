/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"

static volatile odo_status stat;

int main(void){
	system_init();
	debug_init();

	odometry_init();

	while(1){
		odometry_updateStatus_async();
		stat = odometry_getStatus();
		debug_printf("angle: %f revolution: %i speed: %f\n", stat.left.angle, stat.left.revolutions, stat.left.speed);
		LL_mDelay(100);
	}
}
