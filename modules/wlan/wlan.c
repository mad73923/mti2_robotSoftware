/*
 * wlan.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "wlan.h"

void WLANinit(){
	ESP8266init();
}

void WLANcreateAP(const char* SSID,const char* PW){
	ESP8266createAP(SSID,PW);
}
