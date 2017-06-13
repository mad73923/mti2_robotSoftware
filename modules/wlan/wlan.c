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

uint8_t WLANconnectToAp(const char* SSID,const char* PW,const char* IP){
	return ESP8266connectToAp(SSID,PW,IP);
}

uint8_t WLANconnectToTCPserver(const char* IP, const char* Port){
	return ESP8266connectToTCPserver(IP, Port);
}

uint8_t WLANsendTCPmessage(const char* Message){
	return ESP8266sendTCPmessage(Message);
}

uint8_t WLANhandleTCP(void){
	return ESP8266handleTCP();
}
