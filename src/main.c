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
	WLANinit();
	odometry_init();
	motor_timer_init();

	WLANconnectToAp("RobotStation","StationPassword","10.42.0.10");
	while(WLANgetWLANConnectionStatus()!=1){

	}
	WLANconnectToTCPserver("10.42.0.143","2323");
	while(WLANgetTCPConnectionStatus()!=1){

	}
	while(1){

	}
}
