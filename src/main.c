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
	WLANconnectToAp("RobotStation","StationPassword","10.42.0.10");
	while(WLANgetWLANConnectionStatus()!=1){

	}
	WLANconnectToTCPserver("10.42.0.143","2323");
	while(WLANgetTCPConnectionStatus()!=1){

	}
	odometry_init();
	motor_timer_init();
	env_timer_init();
	servo_timer_init();
	adc_init();
	start_env_data_collector();


	while(1){
		odo_status stat = odometry_getStatus();
		debug_printf("speed: left: %f right: %f\n", stat.left.speed, stat.right.speed);
	}
}

