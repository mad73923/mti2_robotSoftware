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

uint8_t WLANcreateAP(const char* SSID,const char* PW,const char* IP){
	return ESP8266createAP(SSID,PW,IP);
}

uint8_t WLANconnectToAp(const char* SSID,const char* PW,const char* IP){
	return ESP8266connectToAp(SSID,PW,IP);
}

uint8_t WLANstartServer(const char* Port){
	return ESP8266startServer(Port);
}

uint8_t WLANhandleConnectionsWithHtml(void){
	return ESP8266handleConnectionsWithHtml();
}

const char* WLANgetIncomingTCPdata(void){
	return ESP8266getIncomingTCPdata();
}

uint8_t WLANconnectToTCPserver(const char* IP, const char* Port){
	return ESP8266connectToTCPserver(IP, Port);
}

uint8_t WLANsendUID(const char* UID){
	return ESP8266sendUID(UID);
}
