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
	//Abstandsmesswerte generieren
//	for(uint16_t i =0; i<cntDistanceVal; i++){
//		distances[i]= i*20+10;//rand()%800;
//	}
	system_init();
	debug_init();
	WLANinit();
	WLANconnectToAp("RobotStation","StationPassword","10.42.0.10");
//	WLANconnectToTCPserver("10.42.0.143","2323");
	//uint32_t cnt = 0;
	while(1){
//		int state = WLANhandleTCP();
//		if(state){
//			debug_printf("%d\r\n",state);
//		}
//			//ToDo: TCP- und WLAN-Verbindungsstatus abfragen und Verbindungen wiederherstellen/Robot stoppen
//			//ToDo: Drive commands in Asynchrone TCP-Datenabfrage implementieren
		//debug_printf("main \n\r");
	}
}
