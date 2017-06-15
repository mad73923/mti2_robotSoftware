/*
 * wlan.c
 *
 *  Created on: 17.05.2017
 *      Author: thomas
 */

#include "wlan.h"

void(*wlanreadyCallback)(uint8_t);
void WlanConnectedToApCallback(uint8_t Error);
void WlanConnectedToTCPcallback(uint8_t Error);
void WlanHandledTCPmessageCallback(uint8_t Error);

uint8_t WLANConnectionStatus = 0;
uint8_t TCPConnectionStatus = 0;

void WLANinit(){
	ESP8266init();
}


void WLANconnectToAp(const char* SSID,const char* PW,const char* IP){
	wlanreadyCallback = WlanConnectedToApCallback;
	ESP8266connectToAp(SSID,PW,IP,wlanreadyCallback);
}

void WLANconnectToTCPserver(const char* IP, const char* Port){
	wlanreadyCallback = WlanConnectedToTCPcallback;
	ESP8266connectToTCPserver(IP, Port,wlanreadyCallback);
}

void WLANstartTCPlistener(){
	wlanreadyCallback = WlanHandledTCPmessageCallback;
	ESP8266startTCPlistener(wlanreadyCallback);
}

uint8_t WLANsendTCPmessage(const char* Message){
	return ESP8266sendTCPmessage(Message);
}

uint8_t WLANhandleTCP(void){
	return ESP8266handleTCP();
}



void WlanConnectedToApCallback(uint8_t Error){
	if(Error){
		debug_printf("WLAN connection failed! \n\r");
		WLANConnectionStatus = 0;
	}
	else{
		WLANConnectionStatus = 1;
		debug_printf("WLAN connection ready! \n\r");
	}
}

uint8_t WLANgetWLANConnectionStatus(){
	return WLANConnectionStatus;
}

void WlanConnectedToTCPcallback(uint8_t Error){
	if(Error){
		debug_printf("TCP connection failed! \n\r");
		TCPConnectionStatus = 0;
	}
	else{
		TCPConnectionStatus = 1;
		debug_printf("TCP connection ready! \n\r");
	}
}

uint8_t WLANgetTCPConnectionStatus(){
	return TCPConnectionStatus;
}

void WlanHandledTCPmessageCallback(uint8_t Error){
	if(Error){
		debug_printf("Handle TCP-Message failed! \n\r");
	}
	else{
		TCPConnectionStatus = 1;
		WLANstartTCPlistener();
	}
}
