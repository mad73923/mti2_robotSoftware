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

uint8_t WLANcreateAP(const char* SSID,const char* PW){
	return ESP8266createAP(SSID,PW);
}
