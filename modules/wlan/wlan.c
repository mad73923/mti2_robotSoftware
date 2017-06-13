/*
 * wlan.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "wlan.h"

void(*wlanreadyCallback)(uint8_t);
void WlanConnectedToApCallback(uint8_t Error);

uint8_t ConnectionStatus = 0;

void WLANinit(){
	ESP8266init();
}


uint8_t WLANconnectToAp(const char* SSID,const char* PW,const char* IP){
	wlanreadyCallback = WlanConnectedToApCallback;
	return ESP8266connectToAp(SSID,PW,IP,wlanreadyCallback);
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

void WlanConnectedToApCallback(uint8_t Error){
	if(Error){
		ConnectionStatus = 0;
	}
	else{
		ConnectionStatus = 1;
	}
}

uint8_t WLANgetConnectionStatus(){
	debug_printf("CONNECT TO AP EVERYTHING OK!\n\r");
	return ConnectionStatus;
}
