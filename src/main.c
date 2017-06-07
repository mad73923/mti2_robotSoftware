/*
 * main.c
 *
 *  Created on: 10.05.2017
 *      Author: matthias
 */

#include "main.h"

uint16_t distances[36]; //Werte im Bereich von 0mm bis 800 mm
uint16_t cntDistanceVal = 36;
uint32_t xPos = 123;
uint32_t yPos = 345;
float angle = 90.1;


int main(void){
	//Abstandsmesswerte generieren
	for(uint16_t i =0; i<cntDistanceVal; i++){
		distances[i]= rand()%800;
	}
	system_init();
	debug_init();
	WLANinit();
	//WLANcreateAP("Robot1","Password","192.168.180.1");
	WLANconnectToAp("RobotStation","StationPassword","192.168.179.110");
	WLANconnectToTCPserver("192.168.179.36","2323");
	//WLANstartServer("80");
	//uint32_t cnt = 0;
	while(1){
		//WLANhandleConnectionsWithHtml();	//Antwortet Anfragen mit einer HTML-Page
		const char* ptrMessage = WLANgetIncomingTCPdata();
		if(ptrMessage!=NULL){
			debug_printf("Data revieved: %s\n\r", ptrMessage );
			if(strcmp(ptrMessage,"GetUID?")==0){
				debug_printf("Robot was asked for UID\n\r");
				WLANsendUID(uid_getUIDString());
			}
			if(strcmp(ptrMessage,"GetPos?")==0){
				debug_printf("Robot was asked for Distances\n\r");
				WLANsendPos(xPos,yPos,angle);
			}
			if(strcmp(ptrMessage,"GetDistances?")==0){
				debug_printf("Robot was asked for Distances\n\r");
				WLANsendDistances(distances,cntDistanceVal);
			}
		}

		//debug_printf("test %d\n", cnt++);
		//LL_mDelay(500);
	}
}
