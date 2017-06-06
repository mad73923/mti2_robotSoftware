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
	//WLANcreateAP("Robot1","Password","192.168.180.1");
	WLANconnectToAp("RobotStation","StationPassword","192.168.179.110");
	//WLANstartServer("80");
	//uint32_t cnt = 0;
	while(1){
		//WLANhandleConnectionsWithHtml();	//Antwortet Anfragen mit einer HTML-Page
		WLANgetIncomingTCPdata();

		//debug_printf("test %d\n", cnt++);
		//LL_mDelay(500);
	}
}
