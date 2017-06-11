/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"

static volatile odo_angles ang;
static volatile odo_revs revs;

int main(void){
	system_init();
	debug_init();

	odometry_init();

	while(1){
		odometry_updateAngles();
		revs = odometry_getRevolutions();
		ang = odometry_getAngles();

		debug_printf("angle: %f revolution: %i\n", ang.angleWheel_l, revs.revsWheel_l);
		LL_mDelay(100);
	}
}
