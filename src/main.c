/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"

uint16_t distances[36]; //Werte im Bereich von 0mm bis 800 mm
uint16_t cntDistanceVal = 36;
int32_t xPos = 123;
int32_t yPos = 345;
float angle = 90;
int32_t DriveStraightVal = 0;
int16_t DriveTurnVal = 0;



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
	WLANstartTCPlistener();
	while(1){

	}
}
