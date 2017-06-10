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
	for(uint16_t i =0; i<cntDistanceVal; i++){
		distances[i]= i*20+10;//rand()%800;
	}
	system_init();
	debug_init();
	WLANinit();
	//WLANcreateAP("Robot1","Password","192.168.180.1");
	//WLANstartServer("80");
	WLANconnectToAp("RobotStation","StationPassword","10.42.0.10");
	WLANconnectToTCPserver("10.42.0.143","2323");
	//uint32_t cnt = 0;
	while(1){
		//WLANhandleConnectionsWithHtml();	//Antwortet Anfragen mit einer HTML-Page
		int state = WLANhandleTCP();
		if(state){
			debug_printf("%d\r\n",state);
		}
//		const char* ptrMessage = WLANgetIncomingTCPdata();
//		if(ptrMessage!=NULL){
//			debug_printf("Data revieved: %s\n\r", ptrMessage );
//			if(strcmp(ptrMessage,"GetUID?")==0){
//				debug_printf("Robot was asked for UID\n\r");
//				WLANsendUID(uid_getUIDString());
//			}
//			if(strcmp(ptrMessage,"GetPos?")==0){
//				debug_printf("Robot was asked for Distances\n\r");
//				WLANsendPos(xPos,yPos,angle);
//			}
//			if(strcmp(ptrMessage,"GetDistances?")==0){
//				debug_printf("Robot was asked for Distances\n\r");
//				WLANsendDistances(distances,cntDistanceVal);
//			}
//			if(strncmp(ptrMessage,"DriveStraight!",14)==0){	//nur 14 Zeichen vergleichen im Anschluss kommt ein Zahlenwert
//				DriveStraightVal=atoi(ptrMessage+14);
//				debug_printf("Robot shoud drive %d mm\n\r", DriveStraightVal);
//				WLANsendTCPmessage("DriveStraight=OK");
//			}
//			if(strncmp(ptrMessage,"DriveTurn!",10)==0){	//nur 10 Zeichen vergleichen im Anschluss kommt ein Zahlenwert
//				//debug_printf("Robot was asked to turn\n\r");
//				DriveTurnVal=strtod(ptrMessage+10,NULL);
//				debug_printf("Robot shoud turn %d degress\n\r", DriveTurnVal);
//				WLANsendTCPmessage("DriveTurn=OK");
//			}
//			//ToDo: TCP- und WLAN-Verbindungsstatus abfragen und Verbindungen wiederherstellen/Robot stoppen
//			//ToDo: Drive commands in Asynchrone TCP-Datenabfrage implementieren
//		}

		//debug_printf("test %d\n", cnt++);
		//LL_mDelay(500);
	}
}
